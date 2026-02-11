#include <iostream>
using namespace std;

#define TAM 10  

class Pila {
private:
    int elemento[TAM];
    int indice;

public:
    Pila() {
        indice = -1;
    }

    bool estaVacia() {
        return (indice == -1);
    }

    bool estaLlena() {
        return (indice == TAM - 1);
    }

    void push(int valor) {
        if (!estaLlena()) {
            indice++;
            elemento[indice] = valor;
        } else {
            cout << "Pila llena" << endl;
        }
    }

    void pop() {
        if (!estaVacia()) {
            indice--;
        } else {
            cout << "Pila vacia" << endl;
        }
    }

    void clean() {
        indice = -1;
    }

    void imprimir() {
        if (estaVacia()) {
            cout << "Pila vacia" << endl;
            return;
        }

        for (int i = 0; i <= indice; i++) {
            cout << elemento[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    Pila pila;

    cout << "Agregndo datos a la Pila...\n";
    pila.push(5);
    pila.imprimir();
    pila.push(10);
    pila.imprimir();
    pila.push(15);
    pila.imprimir();
    pila.push(20);
    pila.imprimir();
    
    cout << "\n---Elementos en la pila: ";
    pila.imprimir();

    pila.pop();
    cout << "\n---Elementos en la pila despues de un pop: ";
    pila.imprimir();

    return 0;
}



// TODO convertir a cola
