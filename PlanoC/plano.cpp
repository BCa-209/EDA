#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include <limits>
using namespace std;

class Plano {
private:
    int maxX;  // Horizontal
    int maxY;  // Vertical

    struct Punto {
        char nombre; // 
        int x;
        int y;
    };

    vector<Punto> puntos;

public:

    void limites(int x, int y) {
        maxX = x;
        maxY = y;
    }

    void agregarPunto(char nombre, int x, int y) {

        // Validar rango
        if(x < 0 || x > maxX || y < 0 || y > maxY) {
            cout << "Error: Punto (" << x << "," << y 
                << ") fuera del rango permitido [0," 
                << maxX << "] x [0," << maxY << "]\n";
            return;
        }

        // Validar nombre repetido
        for(const auto& p : puntos) {
            if(p.nombre == nombre) {
                cout << "Ya existe un punto con nombre " << nombre << endl;
                return;
            }
        }

        for(const auto& p : puntos) {
            if(p.x == x && p.y == y) {
                cout << "Ya existe un punto en esa posicion\n";
                return;
            }
        }

        puntos.push_back({nombre, x, y});
    }

    void distanciaEntrePuntos(char n1, char n2) {
        Punto* p1 = nullptr;
        Punto* p2 = nullptr;
        for(auto& p : puntos) {
            if(p.nombre == n1) p1 = &p;
            if(p.nombre == n2) p2 = &p;
        }
        if(!p1 || !p2) {
            cout << "\n=== Uno o ambos puntos no existen ===\n";
            return;
        }
        double dx = p1->x - p2->x;
        double dy = p1->y - p2->y;
        double distancia = sqrt(dx*dx + dy*dy);
        cout << "\n=== Distancia entre " << n1 << " y " << n2 
            << " es: " << distancia << " === \n";
    }

    void vecinosOrdenados(int x, int y) {

        if(puntos.empty()) {
            cout << "No hay puntos en el plano\n";
            return;
        }

        // Vector auxiliar: (distancia, Punto)
        vector<pair<double, Punto>> distancias;

        for(const auto& p : puntos) {
            double dist = sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
            distancias.push_back({dist, p});
        }

        // Ordenar por distancia (ascendente)
        sort(distancias.begin(), distancias.end(),
            [](const pair<double, Punto>& a,
                const pair<double, Punto>& b) {
                    return a.first < b.first;
            });

        cout << "\nPuntos ordenados por cercania a ("
            << x << "," << y << "):\n";

        for(const auto& d : distancias) {
            cout << d.second.nombre
                << " (" << d.second.x
                << "," << d.second.y
                << ") -> Distancia: "
                << d.first << endl;
        }
    }

    // TODO: input(x,y,Nombre;Grupo)
    // hallar disancia promedio entre los puntos al nuevo y elegir el grupo mas cercano al nuevo punto para asignarlo a ese grupo
    // mostrar los grupos por coloresx
    // ejemplo imc( indice de masa corporal) -> bajo peso, normal, sobrepeso, obesidad

    void draw() {
        for(int y = maxY; y >= 0; y--) {
            for(int x = 0; x <= maxX; x++) {
                if(y == 0 && x == 0) {
                    cout << "   +";
                }
                else if(y == 0) {
                    cout << "--";
                }
                else if(x == 0) {
                    cout << setw(2) << y << " |";
                }
                else {
                    char simbolo = '.';
                    for(const auto& p : puntos) {
                        if(p.x == x && p.y == y) {
                            simbolo = p.nombre;
                            break;
                        }
                    }
                    cout << " " << simbolo;
                }
            }
            cout << endl;
        }

        // Numeración eje X
        cout << "    ";
        for(int x = 1; x <= maxX; x++) {
            if(x % 2 == 0)
                cout << setw(2) << x;
            else
                cout << "  ";
        }
        cout << endl;
    }

    void pntCercano(int x, int y) {
        if(puntos.empty()) {
            cout << "No hay puntos en el plano\n";
            return;
        }

        Punto masCercano = puntos[0];
        double minDist = sqrt(pow(masCercano.x - x, 2) + pow(masCercano.y - y, 2));

        for(const auto& p : puntos) {
            double dist = sqrt(pow(p.x - x, 2) + pow(p.y - y, 2)); //raiz((x2-x1)^2 + (y2-y1)^2)
            if(dist < minDist) {
                minDist = dist;
                masCercano = p;
            }
        }

        cout << "Punto mas cercano a (" << x << "," << y << ") es (" 
            << masCercano.x << "," << masCercano.y << ") con distancia " 
            << minDist << endl;
    }

bool vacio() const {
    return puntos.empty();
}

void mostrarPuntos() const {
    cout << "Puntos disponibles:\n";
    for(const auto& p : puntos) {
        cout << " - " << p.nombre 
             << " (" << p.x << "," << p.y << ")\n";
    }
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
    Plano plano;
    int opcion;
    plano.limites(20, 10);
    plano.agregarPunto('A', 2, 3); 
    plano.agregarPunto('B', 15, 7); 
    plano.agregarPunto('C', 10, 5); 
    plano.agregarPunto('D', 1, 9);
    plano.agregarPunto('E', 5, 5);
    plano.agregarPunto('F', 5, 5);
    do {
        clean();
        cout << "==== MENU PLANO CARTESIANO ====\n";
        cout << "1. Definir limites del plano\n";
        cout << "2. Agregar punto\n";
        cout << "3. Mostrar plano\n";
        cout << "4. Punto mas cercano\n";
        cout << "5. Distancia entre dos puntos\n";
        cout << "6. Salir\n";
        cout << "Seleccione opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: {
                int x, y;
                cout << "Ingrese limite X: ";
                cin >> x;
                cout << "Ingrese limite Y: ";
                cin >> y;
                plano.limites(x, y);
                break;
            }
            case 2: {
                char nom;
                int px, py;
                cout << "Ingrese nombre del punto: ";
                cin >> nom;
                cout << "Ingrese coordenada X del punto: ";
                cin >> px;
                cout << "Ingrese coordenada Y del punto: ";
                cin >> py;
                plano.agregarPunto(nom, px, py);
                plano.draw();
                cout << "Punto agregado. Presione enter para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
            case 3: {
                plano.draw();
                cout << "Presione enter para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
            case 4: {
                int px, py;
                cout << "Ingrese coordenada X de referencia: ";
                cin >> px;
                cout << "Ingrese coordenada Y de referencia: ";
                cin >> py;
                plano.pntCercano(px, py);
                plano.draw();
                cout << "Presione enter para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
            case 5: {
                if(plano.vacio()) {
                    cout << "No hay puntos registrados.\n";
                    break;
                }

                plano.mostrarPuntos();

                char a, b;
                cout << "\nIngrese nombre del primer punto: ";
                cin >> a;
                cout << "Ingrese nombre del segundo punto: ";
                cin >> b;

                plano.distanciaEntrePuntos(a, b);

                cout << "Presione enter para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
            case 6:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida. Presione enter para continuar...";
                cin.ignore();
                cin.get();
                break;
        }

    } while(opcion != 6);

    return 0;
}

// TODO KNN- vecinos mas cercanos
// TODO: nombres a puntos
// TODO: distancia entre puntos
// TODO: 


/*
plano.agregarPunto(2, 3); 
plano.agregarPunto(15, 7); 
plano.agregarPunto(10, 5); 
plano.agregarPunto(1, 9);
plano.pntCercano(5, 5);


    void draw(int posX = -1, int posY = -1) {
        input(posY, posX);
    }
    
    void draw() {
        for(int i=ejeX; i>=0; i--) {
            for(int j=ejeY; j>=0; j--) {
                if(i==0 && j==ejeY){
                    cout << "   + ";
                }
                else if(i == 0) {
                    cout << "--";
                }   
                else if(j == ejeY) {
                    cout << i << "  | ";
                }
                else{
                    cout << ". ";
                }
            }
            cout << endl;
        }
        cout << "    ";
        for(int j=ejeY; j>=1; j--) {
            cout << " ";
            if ((ejeY - j+1)%2 == 0) {
                cout << (ejeY - j+1);
            }else{
                cout << " ";
            };
        }
        cout << endl;
    }
*/