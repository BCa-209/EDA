#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <windows.h> // Para gotoxy
using namespace std;

class Plano {
private:
    int maxX;  // Horizontal
    int maxY;  // Vertical

    struct Punto {
        int x;
        int y;
    };

    vector<Punto> puntos;

    // Función interna para dibujar en coordenadas específicas
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

public:

    void limites(int x, int y) {
        maxX = x;
        maxY = y;
    }

    void agregarPunto(int x, int y) {
        if(x < 0 || x > maxX || y < 0 || y > maxY) {
            cout << "Punto (" << x << "," << y << ") fuera de rango\n";
            return;
        }
        puntos.push_back({x, y});
    }

    void draw() {
        // Dibujar ejes
        for(int y = 0; y <= maxY; y++) {
            gotoxy(0, maxY - y + 1); // +1 para dejar fila 0 para el menú
            cout << setw(2) << y << " |";
        }

        for(int x = 1; x <= maxX; x++) {
            gotoxy(3 + x*2, maxY + 2); // +2 para dejar margen arriba
            if(x % 2 == 0) cout << x;
            else cout << " ";
        }

        // Dibujar puntos
        for(const auto& p : puntos) {
            gotoxy(3 + p.x*2, maxY - p.y + 1);
            cout << "X";
        }
    }

    void pntCercano(int x, int y) {
        if(puntos.empty()) {
            cout << "No hay puntos en el plano\n";
            return;
        }

        Punto masCercano = puntos[0];
        double minDist2 = pow(masCercano.x - x, 2) + pow(masCercano.y - y, 2);

        for(const auto& p : puntos) {
            double dist2 = pow(p.x - x, 2) + pow(p.y - y, 2);
            if(dist2 < minDist2) {
                minDist2 = dist2;
                masCercano = p;
            }
        }

        cout << "Punto mas cercano a (" << x << "," << y << ") es (" 
             << masCercano.x << "," << masCercano.y << ") con distancia " 
             << fixed << setprecision(2) << sqrt(minDist2) << endl;
    }
};

// Limpiar pantalla
void clean() {
    system("cls");
}

int main() {
    Plano plano;
    int opcion;

    do {
        clean();
        cout << "==== MENU PLANO CARTESIANO ====\n";
        cout << "1. Definir limites del plano\n";
        cout << "2. Agregar punto\n";
        cout << "3. Mostrar plano\n";
        cout << "4. Punto mas cercano\n";
        cout << "5. Salir\n";
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
                int px, py;
                cout << "Ingrese coordenada X del punto: ";
                cin >> px;
                cout << "Ingrese coordenada Y del punto: ";
                cin >> py;
                plano.agregarPunto(px, py);
                plano.draw();
                cout << "\nPunto agregado. Presione enter para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
            case 3: {
                clean();
                plano.draw();
                cout << "\nPresione enter para continuar...";
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
                cout << "Presione enter para continuar...";
                cin.ignore();
                cin.get();
                break;
            }
            case 5:
                cout << "Saliendo...\n";
                break;
            default:
                cout << "Opcion invalida. Presione enter para continuar...";
                cin.ignore();
                cin.get();
                break;
        }

    } while(opcion != 5);

    return 0;
}
