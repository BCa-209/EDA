#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
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

                    bool hayPunto = false;

                    for(const auto& p : puntos) {
                        if(p.x == x && p.y == y) {
                            hayPunto = true;
                            break;
                        }
                    }

                    if(hayPunto)
                        cout << " X";
                    else
                        cout << " .";
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

// TODO KNN- vecinos mas cercanos


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