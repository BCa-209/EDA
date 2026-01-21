#include <iostream>
using namespace std;

class Fraccion {
    private:
        int num;
        int dem;
    public:
        Fraccion(int n = 0, int d = 1) : num(n), dem(d) {
            if (d == 0) {
                cout << "Denominador no puede ser cero. 1 por defecto" << endl;
                dem = 1;
            }
        }
        void mostrar() const {
            if (num == 1 && dem == 1) {
                cout << 1 << endl;
            }
            else {
                cout << num << "/" << dem << endl;
            }
        }

        Fraccion Simplificar() const {
            int a = num;
            int b = dem;
            while (b != 0) {
                int temp = b;
                b = a % b;
                a = temp;
            }
            return Fraccion(num / a, dem / a);
        }

        Fraccion operator+(const Fraccion& otra) const {
            return Fraccion(num * otra.dem + otra.num * dem, dem * otra.dem);
        }

        Fraccion operator-(const Fraccion& otra) const {
            return Fraccion(num * otra.dem - otra.num * dem, dem * otra.dem);
        }

        Fraccion operator*(const Fraccion& otra) const {
            return Fraccion(num * otra.num, dem * otra.dem);
        }

        Fraccion operator/(const Fraccion& otra) const {
            return Fraccion(num * otra.dem, dem * otra.num);
        }
    };

int main() {
    Fraccion f1(0, 4);
    Fraccion f2(5, 5);

    cout << "Fracciones Ejemplo:" << endl;
    f1.mostrar();
    f2.mostrar();
    
    for (int i=0; i<10; i++){
        cout << "=";
    }
    cout << endl;

    cout << "\tOperaciones:" << endl;
    cout << "SUma:" << endl;
    Fraccion suma = f1 + f2;
    suma.mostrar();
    cout << endl;
    
    cout << "Resta:" << endl;
    Fraccion resta = f1 - f2;
    resta.mostrar();
    cout << endl;
    
    cout << "Multiplicacion:" << endl;
    Fraccion multiplicacion = f1 * f2;
    multiplicacion.mostrar();
    cout << endl;
    
    cout << "Division:" << endl;
    Fraccion division = f1 / f2;
    division.mostrar();
    cout << endl;
    



}
