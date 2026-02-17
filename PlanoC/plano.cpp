#include <iostream>
#include <iomanip>
using namespace std;

class Plano {
private:
    int maxX;  // Horizontal (eje X)
    int maxY;  // Vertical   (eje Y)

public:

    void limites(int x, int y) {
        maxX = x;  // ancho
        maxY = y;  // alto
    }

    void input(int posX, int posY) {

        // Validación de límites
        if(posX < 0 || posX > maxX || posY < 0 || posY > maxY) {
            cout << "Coordenadas fuera de rango\n";
            return;
        }

        // Recorre eje vertical (Y)
        for(int y = maxY; y >= 0; y--) {
            // Recorre eje horizontal (X)
            for(int x = 0; x <= maxX; x++) {
                // Esquina de los ejes
                if(y == 0 && x == 0) {
                    cout << "   +";
                }
                // Eje horizontal
                else if(y == 0) {
                    cout << "--";
                }
                // Eje vertical
                else if(x == 0) {
                    cout << setw(2) << y << " |";
                }
                // Punto X
                else if(x == posX && y == posY) {
                    cout << " X";
                }
                // Espacio vacío
                else {
                    cout << " .";
                }
            }
            cout << endl;
        }

        // Numeración eje X
        cout << " 0  ";
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

    Plano planito;

    planito.limites(30, 10);  // 20 en X, 10 en Y

    planito.input(2, 3);
    cout << endl;
    planito.input(15, 7);
    cout << endl;

    return 0;
}


// TODO KNN- vecinos mas cercanos


    //void draw(int posX = -1, int posY = -1) {
    //    input(posY, posX);
    //}
    /*
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