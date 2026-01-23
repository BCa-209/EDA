#include <iostream>
using namespace std;

// Clase que representa un nodo de la lista
class Nodo {
public:
    int dato;  
    Nodo* anterior;            // Valor almacenado en el nodo
    Nodo* siguiente;       // Puntero al siguiente nodo

    // Constructor del nodo
    Nodo(int valor) {
        dato = valor;
        anterior = nullptr;
        siguiente = nullptr;
    }
};

// Clase que representa la lista simplemente enlazada
class Lista {
private:
    Nodo* cabezaptr;       // Puntero al primer nodo de la lista
        Nodo* pieptr;       // Puntero al ultimo nodo de la lista

public:
    // Constructor de la lista
    Lista() {
        cabezaptr = nullptr; // La lista inicia vacia
    }

    // Agrega al INICIO (Push Front) ---
    void addIni(int valor) {
        Nodo* nuevo = new Nodo(valor);
        nuevo->siguiente = cabezaptr;

        if( cabezaptr != nullptr) {
            cabezaptr->anterior = nuevo;
        }
        cabezaptr = nuevo;
    }

    // Agrega al FINAL (Push Back / Append) ---
    void add(int valor) {
        Nodo* nuevo = new Nodo(valor); 

        // Caso 1: Si la lista está vacia, el nuevo es la cabeza
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
        nuevo->anterior = temp; // nuevo: apunta al ultimo nodo
    }

    void addPos(int valor, int pos) {
        if (pos < 0) return;
        if (pos == 0) {
            addIni(valor); // Usamos addIni aqui
            return;
        }
        Nodo* aux = cabezaptr;
        for (int i = 0; i < pos - 1; i++) {
            if (aux == nullptr) return;
            aux = aux->siguiente;
        }
        if (aux == nullptr) return;
        
        Nodo* nuevo = new Nodo(valor);
        Nodo* siguienteNodo = aux->siguiente;

        // 1. conectar nuevo con aux (anteroir)
        nuevo->siguiente = siguienteNodo;
        nuevo->anterior = aux;

        // 1. conectar aux con nuevo (siguiente)
        aux->siguiente = nuevo;

        // 1. si habia signte , conectar con nuvo (atras)
        if(siguienteNodo != nullptr) {
            siguienteNodo->anterior = nuevo;
        }
    }

    // Eliminar el PRIMER nodo de la lista
    void dropIni() {
        if (cabezaptr == nullptr) return;
        Nodo* temp = cabezaptr;          // Guardamos el nodo a eliminar
        cabezaptr = cabezaptr->siguiente; // Movemos la cabeza

        // el nuevo no tenra anterior
        if (cabezaptr != nullptr) {
            cabezaptr->anterior = nullptr;
        }
        
        // Si la lista está vacia, no hay nada que eliminar
        if (cabezaptr == nullptr) {
            cout << "Lista vacia, no se puede eliminar el inicio." << endl;
            return;
        }
        delete temp;                     // Liberamos memoria
    }

    // Eliminar el uLTIMO nodo de la lista
    void drop() {

        // Caso A: lista vacia
        if (cabezaptr == nullptr) return;

        // Caso B: solo hay un nodo
        if (cabezaptr->siguiente == nullptr) {
            delete cabezaptr;
            cabezaptr = nullptr;
            return;
        }

        // Caso C: recorrer hasta el penultimo nodo
        Nodo* temp = cabezaptr;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }

        temp->anterior->siguiente = nullptr;

        delete temp;
    }

    // Eliminar un nodo en una POSICIoN especifica
    void dropPos(int pos) {
        // Validaciones
        if (cabezaptr == nullptr || pos < 0) return;
        // Si se quiere eliminar la posicion 0
        if (pos == 0) {
            dropIni();
            return;
        }

        Nodo* actual = cabezaptr;
        for (int i=0; i<pos; i++) {
            if( actual == nullptr) return;
            actual = actual->siguiente;
        }

        if( actual == nullptr) return;

        Nodo* nodoAnterior = actual->anterior;
        Nodo* nodoSiguiente = actual->siguiente;

        // Saltamos el nodo a eliminar
        if ( nodoAnterior != nullptr) {
            nodoAnterior->siguiente= nodoSiguiente;
        }

        if ( nodoSiguiente != nullptr) {
            nodoSiguiente->anterior= nodoAnterior;
        }

        // Liberamos la memoria
        delete actual;
    }

    // Imprimir normal (Izquierda a Derecha)
    void print() {
        cout << "Normal:  NULL <- ";
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            cout << "[ " << actual->dato << " ] <-> ";
            actual = actual->siguiente;
        }
        cout << "NULL" << endl;
    }

    // Imprimir al revés (Para probar que los punteros 'anterior' funcionan)
    void printRev() {
        if (cabezaptr == nullptr) return;

        // 1. Ir al final
        Nodo* actual = cabezaptr;
        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }

        // 2. Imprimir hacia atrás
        cout << "Reversa: NULL <- ";
        while (actual != nullptr) {
            cout << "[ " << actual->dato << " ] <-> ";
            actual = actual->anterior;
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

    // Llenamos la lista
    liss.add(50);
    liss.add(40);
    liss.add(30);
    liss.add(20);
    liss.add(10);

    cout << "--- Lista Inicial ---" << endl;
    liss.print();

    cout << "\nInsertando 99 en la posicion 2..." << endl;
    liss.addPos(99, 2);
    liss.print();

    cout << "\nInsertando 5 en la posicion 4..." << endl;
    liss.addPos(5, 4);
    liss.print();

    cout << "\nInsertando 25 al inicio..." << endl;
    liss.addIni(25);
    liss.print();

    cout << "\nUsando dropIni() (elimina el primer nodo):" << endl;
    liss.dropIni();
    liss.print();

    cout << "\nUsando drop() (elimina el ultimo nodo):" << endl;
    liss.drop();
    liss.print();

    cout << "\nUsando dropPos(2) (elimina el nodo en indice 2):" << endl;
    liss.dropPos(2);
    liss.print();
    liss.printRev();
    return 0;
}

// TODO desturctor, agregar por posicion, eliminar por posicion.

// TODDO pila y cola con listas doblemente eslanzadas
// TODDO lista doblemenrerni elnlashgascdt