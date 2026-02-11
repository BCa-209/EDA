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

class Pila {
private:
    Lista elementos = Lista();
public:
    void enpilar(int valor) {
        elementos.addIni(valor);
    }

    void despilar() {
        elementos.dropIni();    
    }
    void mostrarPila() {
        elementos.print();
    }
};

class Cola {
private:
    Lista elementos = Lista();
public:
    void encolar(int valor) {
        elementos.add(valor);
    }
    void desencolar() {
        elementos.dropIni();    
    }
    void mostrarCola() {
        elementos.print();
    }
};

int main() {
    cout << "--------AHORA CON COLA---------" << endl;
    // Llenamos la lista
    Cola cola;
    cola.encolar(1);
    cola.mostrarCola();
    cola.encolar(2);
    cola.mostrarCola();
    cola.encolar(3);    // 1 -> 2 -> 3
    cola.mostrarCola();
    cola.desencolar();  // 2 -> 3
    cola.mostrarCola();


    return 0;
}

// TODO pila y cola con listas doblemente enlazadas