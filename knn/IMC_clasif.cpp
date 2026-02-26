#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <limits>

using namespace std;

class Clasificador {
private:
    int maxX;
    int maxY;

    struct Muestra {
        char nombre;
        int x;
        int y;
        int grupo;
    };

    vector<Muestra> muestras;

    // Colores ANSI
    string color(int grupo) const {
        const vector<string> colores = {
            "\033[31m", // rojo
            "\033[32m", // verde
            "\033[34m", // azul
            "\033[35m", // magenta
            "\033[36m", // cyan
            "\033[33m"  // amarillo
        };
        return colores[(grupo - 1) % colores.size()];
    }

public:

    void limites(int x, int y) {
        maxX = x;
        maxY = y;
    }

    bool vacio() const {
        return muestras.empty();
    }

    void mostrarPuntos() const {
        cout << "\nMuestras disponibles:\n";
        for(const auto& m : muestras) {
            cout << " - " << m.nombre 
                << " (" << m.x << "," << m.y 
                << ") Grupo: " << m.grupo << "\n";
        }
    }

    void agregarPunto(char nombre, int x, int y, int grupo) {

        if(x < 0 || x > maxX || y < 0 || y > maxY) {
            cout << "Error: Muestra fuera de rango\n";
            return;
        }

        for(const auto& m : muestras) {
            if(m.nombre == nombre) {
                cout << "Ya existe una muestra con ese nombre\n";
                return;
            }
            if(m.x == x && m.y == y) {
                cout << "Ya existe una muestra en esa posicion\n";
                return;
            }
        }

        muestras.push_back({nombre, x, y, grupo});
    }

    int grupoMasCercano(int x, int y) {

        if(muestras.empty())
            return 1;

        map<int, pair<double,int>> acumulado;

        for(const auto& m : muestras) {
            double dx = m.x - x;
            double dy = m.y - y;
            double dist2 = dx*dx + dy*dy;

            acumulado[m.grupo].first += dist2;
            acumulado[m.grupo].second++;
        }

        double mejorProm = numeric_limits<double>::max();
        int mejorGrupo = 1;

        for(const auto& g : acumulado) {
            double promedio = g.second.first / g.second.second;

            if(promedio < mejorProm) {
                mejorProm = promedio;
                mejorGrupo = g.first;
            }
        }

        return mejorGrupo;
    }

    void distanciaEntrePuntos(char n1, char n2) {

        Muestra* p1 = nullptr;
        Muestra* p2 = nullptr;

        for(auto& m : muestras) {
            if(m.nombre == n1) p1 = &m;
            if(m.nombre == n2) p2 = &m;
        }

        if(!p1 || !p2) {
            cout << "Uno o ambos puntos no existen\n";
            return;
        }

        double dx = p1->x - p2->x;
        double dy = p1->y - p2->y;

        cout << "Distancia: " 
            << sqrt(dx*dx + dy*dy) << endl;
    }

    void vecinosOrdenados(int x, int y) {

        if(muestras.empty()) {
            cout << "No hay muestras\n";
            return;
        }

        vector<pair<double, Muestra>> distancias;

        for(const auto& m : muestras) {
            double dist = sqrt(pow(m.x - x, 2) + pow(m.y - y, 2));
            distancias.push_back({dist, m});
        }

        sort(distancias.begin(), distancias.end(),
            [](const pair<double, Muestra>& a,
            const pair<double, Muestra>& b) {
                return a.first < b.first;
        });

        cout << "\nPuntos ordenados por cercania:\n";

        for(const auto& d : distancias) {
            cout << d.second.nombre
                << " (" << d.second.x
                << "," << d.second.y
                << ") -> " << d.first << endl;
        }
    }

    void pntCercano(int x, int y) {

        if(muestras.empty()) {
            cout << "No hay muestras\n";
            return;
        }

        Muestra masCercano = muestras[0];
        double minDist = sqrt(pow(masCercano.x - x, 2) +
                            pow(masCercano.y - y, 2));

        for(const auto& m : muestras) {
            double dist = sqrt(pow(m.x - x, 2) +
                            pow(m.y - y, 2));

            if(dist < minDist) {
                minDist = dist;
                masCercano = m;
            }
        }

        cout << "Mas cercano: "
            << masCercano.nombre
            << " Distancia: " << minDist << endl;
    }

    void draw() {

        for(int y = maxY; y >= 0; y--) {
            for(int x = 0; x <= maxX; x++) {

                if(y == 0 && x == 0)
                    cout << "   +";
                else if(y == 0)
                    cout << "--";
                else if(x == 0)
                    cout << setw(2) << y << " |";
                else {

                    char simbolo = '.';
                    int grupo = 0;

                    for(const auto& m : muestras) {
                        if(m.x == x && m.y == y) {
                            simbolo = m.nombre;
                            grupo = m.grupo;
                            break;
                        }
                    }

                    if(simbolo != '.')
                        cout << color(grupo)
                            << " " << simbolo
                            << "\033[0m";
                    else
                        cout << " .";
                }
            }
            cout << endl;
        }

        cout << "    ";
        for(int x = 1; x <= maxX; x++) {
            if(x % 2 == 0)
                cout << setw(2) << x;
            else
                cout << "  ";
        }
        cout << endl;
    }

    int knn(int x, int y, int K) {

        if(muestras.empty()) {
            cout << "No hay muestras en el plano.\n";
            return -1;
        }
        // Si K es -1 o mayor que la cantidad de puntos = usar todos
        if(K == -1 || K > muestras.size()) {
            K = muestras.size();
        }
        if(K <= 0) {
            cout << "K debe ser mayor que 0 o -1 para usar todos.\n";
            return -1;
        }

        vector<pair<double,int>> distancias;

        // Calcular distancia a todos los puntos
        for(const auto& m : muestras) {
            double dx = m.x - x;
            double dy = m.y - y;
            double dist = sqrt(dx*dx + dy*dy);

            distancias.push_back({dist, m.grupo});
        }

        // Ordenar por distancia (menor a mayor)
        sort(distancias.begin(), distancias.end(),
            [](const pair<double,int>& a,
                const pair<double,int>& b) {
                    return a.first < b.first;
            });

        map<int,int> conteo;

        // Tomar los K vecinos mas cercanos
        for(int i = 0; i < K && i < distancias.size(); i++) {
            conteo[distancias[i].second]++;
        }

        // Buscar grupo con mayor frecuencia
        int mejorGrupo = -1;
        int maxCount = 0;

        for(const auto& g : conteo) {
            if(g.second > maxCount) {
                maxCount = g.second;
                mejorGrupo = g.first;
            }
        }

        return mejorGrupo;
    }

};

int main() {

    Clasificador plano;
    plano.limites(30, 20);

    // Datos de Entrenamiento
    // plano.agregarPunto(NameP, X, Y, Grupo);
    plano.agregarPunto('A', 170, 60, 2);    // normal
    plano.agregarPunto('B', 165, 45, 1);    // bajo peso
    plano.agregarPunto('C', 180, 95, 4);    // obesidad
    plano.agregarPunto('D', 175, 85, 3);    // sobrepeso
    cout << "Grupo 1: A, B, C, D\n";

    // Nuevo punto automatico
    int x = 178;
    int y = 64;
    int grupo1 = plano.grupoMasCercano(x, y);
    
    plano.agregarPunto('F', x, y, grupo1);
    cout << "Punto " << "F" << ". Grupo asignado automaticamente: " << grupo1 << endl;
    
    // Nuevo punto con knn
    int x2 = 15;
    int y2 = 5;
    int K = 7;
    int grupo2 = plano.knn(x2, y2, K);
    if(grupo2 != -1) {
        plano.agregarPunto('M', x2, y2, grupo2);
        cout << "Punto " << "M" << ". Grupo asignado por KNN: " << grupo2 << endl;
    }

    plano.draw();

    return 0;
}
