#include <iostream>
using namespace std;

// Clase que representa un nodo de la lista
class Nodo {
public:
    int dato;              // Valor almacenado en el nodo
    Nodo* siguiente;       // Puntero al siguiente nodo

    // Constructor del nodo
    Nodo(int valor) {
        dato = valor;
        siguiente = nullptr;
    }
};

// Clase que representa la lista simplemente enlazada
class Lista {
private:
    Nodo* cabezaptr;       // Puntero al primer nodo de la lista

public:
    // Constructor de la lista
    Lista() {
        cabezaptr = nullptr; // La lista inicia vacía
    }

    // Agrega al INICIO (Push Front) ---
    void addIni(int valor) {
        Nodo* nuevo = new Nodo(valor);
        nuevo->siguiente = cabezaptr;
        cabezaptr = nuevo;
    }

    // Agrega al FINAL (Push Back / Append) ---
    void add(int valor) {
        Nodo* nuevo = new Nodo(valor); 

        // Caso 1: Si la lista está vacía, el nuevo es la cabeza
        if (cabezaptr == nullptr) {
            cabezaptr = nuevo;
            return;
        }

        // Caso 2: Recorrer hasta el último nodo
        Nodo* temp = cabezaptr;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }

        // Ahora temp es el último nodo, hacemos que apunte al nuevo
        temp->siguiente = nuevo;
    }

    // Resto de funciones igual que antes ---
    
    void addPos(int valor, int pos) {
        if (pos < 0) return;
        if (pos == 0) {
            addIni(valor); // Usamos addIni aquí
            return;
        }
        Nodo* aux = cabezaptr;
        for (int i = 0; i < pos - 1; i++) {
            if (aux == nullptr) return;
            aux = aux->siguiente;
        }
        if (aux == nullptr) return;
        
        Nodo* nuevo = new Nodo(valor);
        nuevo->siguiente = aux->siguiente;
        aux->siguiente = nuevo;
    }

    // Eliminar el PRIMER nodo de la lista
    void dropIni() {

        // Si la lista está vacía, no hay nada que eliminar
        if (cabezaptr == nullptr) {
            cout << "Lista vacía, no se puede eliminar el inicio." << endl;
            return;
        }

        Nodo* temp = cabezaptr;          // Guardamos el nodo a eliminar
        cabezaptr = cabezaptr->siguiente; // Movemos la cabeza
        delete temp;                     // Liberamos memoria
    }

    // Eliminar el uLTIMO nodo de la lista
    void drop() {

        // Caso A: lista vacía
        if (cabezaptr == nullptr) return;

        // Caso B: solo hay un nodo
        if (cabezaptr->siguiente == nullptr) {
            delete cabezaptr;
            cabezaptr = nullptr;
            return;
        }

        // Caso C: recorrer hasta el penultimo nodo
        Nodo* temp = cabezaptr;
        while (temp->siguiente->siguiente != nullptr) {
            temp = temp->siguiente;
        }

        // Eliminamos el ultimo nodo
        delete temp->siguiente;
        temp->siguiente = nullptr;
    }

    // Eliminar un nodo en una POSICIoN específica
    void dropPos(int pos) {

        // Validaciones básicas
        if (cabezaptr == nullptr || pos < 0) return;

        // Si se quiere eliminar la posicion 0
        if (pos == 0) {
            dropIni();
            return;
        }

        Nodo* anterior = cabezaptr;

        // Buscamos el nodo anterior al que se va a eliminar
        for (int i = 0; i < pos - 1; i++) {
            if (anterior->siguiente == nullptr) {
                cout << "Posicion fuera de rango." << endl;
                return;
            }
            anterior = anterior->siguiente;
        }

        Nodo* aEliminar = anterior->siguiente;

        // Verificamos que el nodo exista
        if (aEliminar == nullptr) {
            cout << "Posicion fuera de rango." << endl;
            return;
        }

        // Saltamos el nodo a eliminar
        anterior->siguiente = aEliminar->siguiente;

        // Liberamos la memoria
        delete aEliminar;
    }

    // Imprimir todos los elementos de la lista
    void print() {
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            cout << actual->dato << " -> ";
            actual = actual->siguiente;
        }
        cout << "NULL" << endl;
    }

    // Destructor: libera toda la memoria usada por la lista
    ~Lista() {
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
        cout << "Memoria liberada." << endl;
    }
};

int main() {
    Lista liss;

    // Llenamos la lista: 10 -> 20 -> 30 -> 40 -> 50
    liss.add(10);
    //liss.add(40);
    //liss.add(30);
    //liss.add(20);
    //liss.add(10);

    cout << "--- Lista Inicial ---" << endl;
    liss.print();
    
    cout << "\nInsertando 25 al inicio..." << endl;
    liss.addIni(5);
    liss.print();

    cout << "\nInsertando 7 en la posicion 1..." << endl;
    liss.addPos(7, 1);
    liss.print();

    return 0;
}


// TODO desturctor, agregar por posicion, eliminar por posicion.

// TODDO pila y cola con listas doblemente eslanzadas
// TODDO lista doblemenrerni elnlashgascdt