#include <iostream>
using namespace std;

#define MAX 10

class Cola {
private:
    int elementos[MAX];
    //
    int frente;
    int final;

public:
    Cola() {
        frente = -1;
        final = -1;
    }

    bool estaVacia() {
        return (frente == -1);
    }

    bool estaLlena() {
        return (frente == (final + 1) % MAX);
    }

    void encolar(int valor) {
        if(estaLlena()) {
            cout << "Cola llena" << endl;
        }
        if(estaVacia()) {
            frente = 0;
        }
        
        final = (final + 1) % MAX;
        elementos[final] = valor;
        
    }
    
    int desencolar() {
        if(estaVacia()) {
            cout << "Cola vacia" << endl;
            return -1;
        }
        
        int valor = elementos[frente];
        
        if(frente == final) {
            frente = -1;
            final = -1;
        }
        else {
            frente = (frente + 1) % MAX;
        }
        return valor;
    }

    void imprimir() {
        if (estaVacia()) {
            cout << "Cola vacia" << endl;
            return;
        }

        int i = frente;
        while (true) {
            cout << elementos[i] << " ";
            if (i == final) break;
            i = (i + 1) % MAX;
        }
        cout << endl;
    }
};

int main() {
    Cola cola;

    cola.encolar(5);
    cola.encolar(5);
    cola.encolar(10);
    cola.encolar(10);
    cola.encolar(15);
    cola.encolar(15);
    cola.encolar(10);
    cola.encolar(10);
    cola.encolar(15);
    cola.encolar(15);
    
    cout << "Elementos en la cola: ";
    cola.imprimir();

    //cout << "Desencolando: " << cola.desencolar() << endl;

    cout << "Elementos despues de desencolar: ";
    cola.imprimir();

    return 0;
}