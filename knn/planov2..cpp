#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <limits>

using namespace std;

class Plano {
private:
    int maxX;
    int maxY;

    struct Punto {
        char nombre;
        int x;
        int y;
        int grupo;
    };

    vector<Punto> puntos;

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
        return puntos.empty();
    }

    void mostrarPuntos() const {
        cout << "\nPuntos disponibles:\n";
        for(const auto& p : puntos) {
            cout << " - " << p.nombre 
                 << " (" << p.x << "," << p.y 
                 << ") Grupo: " << p.grupo << "\n";
        }
    }

    void agregarPunto(char nombre, int x, int y, int grupo) {

        if(x < 0 || x > maxX || y < 0 || y > maxY) {
            cout << "Error: Punto fuera de rango\n";
            return;
        }

        for(const auto& p : puntos) {
            if(p.nombre == nombre) {
                cout << "Ya existe un punto con ese nombre\n";
                return;
            }
            if(p.x == x && p.y == y) {
                cout << "Ya existe un punto en esa posicion\n";
                return;
            }
        }

        puntos.push_back({nombre, x, y, grupo});
    }

    // 🔥 Asignar grupo automáticamente
    int grupoMasCercano(int x, int y) {

        if(puntos.empty())
            return 1;

        map<int, pair<double,int>> acumulado;

        for(const auto& p : puntos) {
            double dx = p.x - x;
            double dy = p.y - y;
            double dist2 = dx*dx + dy*dy;

            acumulado[p.grupo].first += dist2;
            acumulado[p.grupo].second++;
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

        Punto* p1 = nullptr;
        Punto* p2 = nullptr;

        for(auto& p : puntos) {
            if(p.nombre == n1) p1 = &p;
            if(p.nombre == n2) p2 = &p;
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

        if(puntos.empty()) {
            cout << "No hay puntos\n";
            return;
        }

        vector<pair<double, Punto>> distancias;

        for(const auto& p : puntos) {
            double dist = sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
            distancias.push_back({dist, p});
        }

        sort(distancias.begin(), distancias.end(),
            [](const pair<double, Punto>& a,
               const pair<double, Punto>& b) {
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

        if(puntos.empty()) {
            cout << "No hay puntos\n";
            return;
        }

        Punto masCercano = puntos[0];
        double minDist = sqrt(pow(masCercano.x - x, 2) +
                              pow(masCercano.y - y, 2));

        for(const auto& p : puntos) {
            double dist = sqrt(pow(p.x - x, 2) +
                               pow(p.y - y, 2));

            if(dist < minDist) {
                minDist = dist;
                masCercano = p;
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

                    for(const auto& p : puntos) {
                        if(p.x == x && p.y == y) {
                            simbolo = p.nombre;
                            grupo = p.grupo;
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

    

};

int main() {

    Plano plano;
    plano.limites(20, 10);

    // Puntos iniciales con grupos
    // plano.agregarPunto(NP, X, Y, Grupo);
    plano.agregarPunto('A', 2, 3, 1);
    plano.agregarPunto('B', 15, 7, 2);
    plano.agregarPunto('C', 10, 5, 2);
    plano.agregarPunto('D', 1, 9, 1);
    plano.agregarPunto('E', 5, 5, 1);

    // Nuevo punto automático
    int x = 8, y = 4;
    int grupo = plano.grupoMasCercano(x, y);

    cout << "Grupo asignado automaticamente: "
         << grupo << endl;

    plano.agregarPunto('F', x, y, grupo);

    plano.draw();

    return 0;
}

    // TODO: input(x,y,Nombre;Grupo)
    // hallar disancia promedio entre los puntos al nuevo y elegir el grupo mas cercano al nuevo punto para asignarlo a ese grupo
    // hallar el gtupo mediante la distancia promedio entre el nuevo punto y los puntos de cada grupo, asignar el nuevo punto al grupo con menor distancia promedio
    // mostrar los grupos por colores
    // ejemplo imc( indice de masa corporal) -> bajo peso, normal, sobrepeso, obesidad

//********----///
    //TODO: Hacer una monografia de la semana 7 del silabo con pseudocodigo y codigo 