#include <iostream>
using namespace std;

#define TAM 10

class Cola {
private:
    int elemento[TAM];
    //
    int frente;
    int final;

public:
    Cola() {
        frente = 0;
        final = -1;
    }

    bool estaVacia() {
        return (final < frente);
    }

    bool estaLlena() {
        return (final == TAM - 1);
    }

    void encolar(int valor) {
        if (!estaLlena()) {
            final++;
            elemento[final] = valor;
        } else {
            cout << "Cola llena" << endl;
        }
    }

    void desencolar() {
        if (!estaVacia()) {
            frente++;
        } else {
            cout << "Cola vacia" << endl;
        }
    }

    void imprimir() {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
            return;
        }

        for (int i = frente; i <= final; i++) {
            cout << elemento[i] << " ";
        }
        cout << endl;
    }

    void limpiar() {
        frente = 0;
        final = -1;
    }
};

int main() {
    Cola cola;

    cola.encolar(5);
    cola.encolar(10);
    cola.encolar(15);

    cout << "Elementos en la cola: ";
    cola.imprimir();

    cola.desencolar();
    cout << "Elementos despues de desencolar: ";
    cola.imprimir();

    return 0;
}
