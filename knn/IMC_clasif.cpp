#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <limits>
#include <cstdlib>

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
void agregarPunto(char nombre, int m, int n, int grupo) {
    int x = m/10;
    int y = n/10;
    if (x < 0 || x > maxX || y < 0 || y > maxY) {
        cout << "Error: Muestra fuera de rango\n";
        return;
    }

    // Verificar que el nombre no se repita
    for (const auto& m : muestras) {
        if (m.nombre == nombre) {
            cout << "Ya existe una muestra con ese nombre\n";
            return;
        }
    }

    // Ahora se permite agregar varios puntos en la misma posicion
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

        // Mapa de posiciones (x, y) con la cantidad de puntos
        map<pair<int, int>, int> contadorPosiciones;

        // Contar cuantos puntos hay en cada posicion
        for (const auto& m : muestras) {
            contadorPosiciones[{m.x, m.y}]++;
        }

        // Dibuja el plano
        for (int y = maxY; y >= 0; y--) {
            for (int x = 0; x <= maxX; x++) {

                if (y == 0 && x == 0)
                    cout << "   +";  // Marca el origen
                else if (y == 0)
                    cout << "--";  // Marca la linea X
                else if (x == 0)
                    cout << setw(2) << y << " |";  // Marca la linea Y
                else {

                    char simbolo = '.';  // Por defecto, un espacio vacio
                    int grupo = 0;
                    int conteo = 0;  // Para contar cuantos puntos hay en esta posicion

                    for (const auto& m : muestras) {
                        if (m.x == x && m.y == y) {
                            simbolo = m.nombre;  // Usamos el nombre del punto
                            grupo = m.grupo;  // Guardamos el grupo
                            conteo++;  // Contamos los puntos en la misma posicion
                        }
                    }

                    // Si hay mas de un punto en la misma posicion
                    if (conteo > 1) {
                        cout << color(grupo) << " *" << conteo << "\033[0m";  // Mostrar un asterisco con el conteo
                    } else if (simbolo != '.') {
                        cout << color(grupo) << " " << simbolo << "\033[0m";  // Mostrar el punto con el color
                    } else {
                        cout << " .";  // Espacio vacio
                    }
                }
            }
            cout << endl;
        }

        // Imprimir los numeros del eje X
        cout << "    ";
        for (int x = 1; x <= maxX; x++) {
            if (x % 2 == 0)
                cout << setw(2) << x;
            else
                cout << "  ";
        }
        cout << endl;
    }

    void listarPuntosPorPosicion() {
        // Mapa para contar puntos en cada posicion (x, y)
        map<pair<int, int>, vector<Muestra>> puntosPorPosicion;

        // Agrupar puntos por sus coordenadas (x, y)
        for (const auto& m : muestras) {
            puntosPorPosicion[{m.x, m.y}].push_back(m);
        }

        // Mostrar los puntos agrupados por posicion
        cout << "Puntos agrupados por posicion:\n";
        for (const auto& entry : puntosPorPosicion) {
            cout << "Posicion (" << entry.first.first << ", " << entry.first.second << "):\n";
            for (const auto& p : entry.second) {
                cout << "  - " << p.nombre << " (Grupo: " << p.grupo << ")\n";
            }
        }
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

void clean() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    clean();
    Clasificador plano;
    plano.limites(30, 20);  // Establecer limites del plano 2D
    // Bajo peso
    plano.agregarPunto('E',150, 45, 1);
    plano.agregarPunto('F',155, 48, 1);
    plano.agregarPunto('G',160, 50, 1);
    plano.agregarPunto('H',158, 47, 1);
    
    // Normal
    plano.agregarPunto('I', 165, 60, 2);
    plano.agregarPunto('J', 170, 65, 2);
    plano.agregarPunto('K', 172, 68, 2);
    plano.agregarPunto('L', 168, 63, 2);
    
    // Sobrepeso
    //plano.agregarPunto('M', 175, 78, 3);
    plano.agregarPunto('N', 178, 82, 3);
    plano.agregarPunto('O', 180, 85, 3);
    plano.agregarPunto('P', 173, 80, 3);
    
    // Obesidad
    plano.agregarPunto('Q', 165, 95, 4);
    plano.agregarPunto('R', 170, 100, 4);
    plano.agregarPunto('S', 160, 92, 4);
    plano.agregarPunto('T', 175, 105, 4);

    bool continuar = true;
    while (continuar) {
        cout << "====================================\n";
        cout << "|| Calcular IMC por clasificacion ||\n";
        cout << "====================================\n";
        cout << "\n----- Menu de Opciones -----\n";
        cout << "1. Agregar una muestra\n";
        cout << "   (Ingresa una muestra con altura, peso y grupo)\n";
        cout << "2. Mostrar grafico 2D de las muestras\n";
        cout << "3. Listar muestras agrupadas por posicion\n";
        cout << "4. Asignar grupo automaticamente a una muestra\n";
        cout << "   (Basado en la distancia promedio al grupo mas cercano)\n";
        cout << "5. Asignar grupo a una muestra por KNN\n";
        cout << "   (Usa el algoritmo KNN con los K puntos mas cercanos)\n";
        cout << "6. Salir del programa\n";
        cout << "Elige una opcion (1-6): ";
        
        int opcion;
        cin >> opcion;

        switch(opcion) {
            case 1: {
                clean();
                char nombre;
                int x, y, grupo;
                cout << "--- Agregar una muestra ---\n";
                cout << "Introduce el nombre del punto: ";
                cin >> nombre;
                cout << "Introduce la altura (cm.): ";
                cin >> x;
                cout << "Introduce el peso (kg.): ";
                cin >> y;
                cout << "Introduce el grupo:\n";
                cout << "-> 1. Bajo peso (IMC < 18.5)\n";
                cout << "-> 2. Normal (18.5 <= IMC < 24.9)\n";
                cout << "-> 3. Sobrepeso (25 <= IMC < 29.9)\n";
                cout << "-> 4. Obesidad (IMC >= 30)\n";
                cin >> grupo;
                plano.agregarPunto(nombre, x, y, grupo);
                break;
            }
            case 2:
                clean();
                cout << "--- Dibujar plano ---\n";
                plano.draw();  // Dibuja el plano
                break;
            case 3:
                clean();
                cout << "--- Listar puntos por posicion ---\n";
                plano.listarPuntosPorPosicion();  // Listar puntos por posicion
                break;
            case 4: {
                clean();
                int x, y;
                cout << "--- Asignar grupo automaticamente ---\n";
                cout << "Introduce las coordenadas (x, y) para asignar un grupo automaticamente: ";
                cin >> x >> y;
                int grupo1 = plano.grupoMasCercano(x, y);
                plano.agregarPunto('F', x, y, grupo1);
                cout << "Punto 'F'. Grupo asignado automaticamente: " << grupo1 << endl;
                break;
            }
            case 5: {
                clean();
                int x, y, K;
                cout << "--- Asignar grupo por KNN ---\n";
                cout << "Introduce las coordenadas (x, y) para KNN: ";
                cin >> x >> y;
                cout << "Introduce el valor de K: ";
                cin >> K;
                int grupo2 = plano.knn(x, y, K);
                if (grupo2 != -1) {
                    plano.agregarPunto('M', x, y, grupo2);
                    cout << "Punto 'M'. Grupo asignado por KNN: " << grupo2 << endl;
                }
                break;
            }
            case 6:
                cout << "¿Estas seguro que quieres salir? (Y/n): ";
                char confirmacion;
                cin >> confirmacion;
                if (confirmacion == 'Y' || confirmacion == 'y') {
                    continuar = false;  // Salir del bucle
                }
                break;
            default:
                cout << "Opcion no valida, intenta nuevamente.\n";
        }

        // Esperar a que el usuario presione Enter antes de continuar
        cout << "\nPresiona Enter para continuar...";
        cin.ignore();  // Limpiar buffer de entrada
        cin.get();     // Esperar entrada
        clean();  // Limpiar pantalla
    }

    return 0;
}
