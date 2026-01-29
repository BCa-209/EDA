#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <queue>

using namespace std;

class Nodo {
public:
    char dato;  
    Nodo* izquierdo;            // Valor almacenado en el nodo
    Nodo* derecho;       // Puntero al siguiente nodo

    // Constructor del nodo
    Nodo(char valor) {
        dato = valor;
        izquierdo = nullptr;
        derecho = nullptr;
    }
};

class Arbol {
public:
    Nodo* raiz;       // Puntero a la raíz del árbol

    // Constructor del árbol
    Arbol() {
        raiz = nullptr; // El árbol inicia vacío
    }


    Nodo* insertar(char valor) {
        raiz = insertarRec(raiz, valor);
        return raiz;
    }

    Nodo* insertarRec(Nodo* nodo, char valor) {
        if (nodo == nullptr) {
            return new Nodo(valor);
        }

        if (valor < nodo->dato) {
            nodo->izquierdo = insertarRec(nodo->izquierdo, valor);
        }
        else {
            nodo->derecho = insertarRec(nodo->derecho, valor);
        }
        return nodo;
    }

    Nodo* buscar(int valor) {
        return buscarRec(raiz, valor);
    }

    Nodo* buscarRec(Nodo* nodo, int valor) {
        if (nodo == nullptr) {
            return nullptr;
        }

        if (nodo->dato == valor) {
            return nodo;
        }

        if (valor < nodo->dato) {
            return buscarRec(nodo->izquierdo, valor);
        }
        else {
            return buscarRec(nodo->derecho, valor);
        }
    }

    Nodo* eliminar(int valor) {
        raiz = eliminarRec(raiz, valor);
        return raiz;
    }

    Nodo* eliminarRec(Nodo* nodo, int valor) {
        if (nodo == nullptr) {
            return nodo;
        }

        if (valor < nodo->dato) {
            nodo->izquierdo = eliminarRec(nodo->izquierdo, valor);
        }
        else if (valor > nodo->dato) {
            nodo->derecho = eliminarRec(nodo->derecho, valor);
        }
        else {
            // Nodo a eliminar encontrado
            if (nodo->izquierdo == nullptr) {
                Nodo* temp = nodo->derecho;
                delete nodo;
                return temp;
            }
            else if (nodo->derecho == nullptr) {
                Nodo* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }

            // Nodo con dos hijos
            Nodo* temp = encontrarMinimo(nodo->derecho);
            nodo->dato = temp->dato;
            nodo->derecho = eliminarRec(nodo->derecho, temp->dato);
        }
        return nodo;
    }

    Nodo* encontrarMinimo(Nodo* nodo) {
        while (nodo != nullptr && nodo->izquierdo != nullptr) {
            nodo = nodo->izquierdo;
        }
        return nodo;
    }

    void preOrder() {
        preOrderRec(raiz);
    }
    void preOrderRec(Nodo* nodo) {
        if (nodo != nullptr) {
            cout << nodo->dato << " ";
            preOrderRec(nodo->izquierdo);
            preOrderRec(nodo->derecho);
        }
    }

    void inOrder() {
        inOrderRec(raiz);
    }
    void inOrderRec(Nodo* nodo) {
        if (nodo != nullptr) {
            inOrderRec(nodo->izquierdo);
            cout << nodo->dato << " ";
            inOrderRec(nodo->derecho);
        }
    }

    void postOrder() {
        postOrderRec(raiz);
    }
    void postOrderRec(Nodo* nodo) {
        if (nodo != nullptr) {
            postOrderRec(nodo->izquierdo);
            postOrderRec(nodo->derecho);
            cout << nodo->dato << " ";
        }
    }

    void imprimir(Nodo* nodo, int nivel = 0) {
        if (nodo == nullptr)
            return;

        // Primero el subárbol derecho
        imprimir(nodo->derecho, nivel + 1);

        // Espacios según el nivel
        for (int i = 0; i < nivel; i++)
            cout << "    ";

        // Imprimir el dato
        cout << nodo->dato << endl;

        // Luego el subárbol izquierdo
        imprimir(nodo->izquierdo, nivel + 1);
    }

    void imprimirArbol(Nodo* nodo, string prefijo = "", bool esIzq = true) {
        if (nodo == nullptr) return;

        cout << prefijo;
        cout << (esIzq ? "|-- " : "\\-- ");
        cout << nodo->dato << endl;

        imprimirArbol(nodo->izquierdo, prefijo + (esIzq ? "|   " : "    "), true);
        imprimirArbol(nodo->derecho, prefijo + (esIzq ? "|   " : "    "), false);
    }

    void visualizarHorizontal() {
        if (raiz == nullptr) {
            cout << "El árbol está vacío." << endl;
            return;
        }
        
        int altura = calcularAltura(raiz);
        int ancho = pow(2, altura + 1) - 1;
        
        vector<vector<string>> niveles(altura * 2);
        vector<vector<bool>> tieneHijo(altura * 2);
        
        // Inicializar matrices
        for (int i = 0; i < altura * 2; i++) {
            niveles[i].resize(ancho, "   ");
            tieneHijo[i].resize(ancho, false);
        }
        
        // Llenar la matriz con los valores del árbol
        llenarMatriz(raiz, niveles, tieneHijo, 0, ancho / 2, altura, 0);
        
        // Imprimir el árbol
        cout << "\nARBOL BINARIO (Formato Horizontal):\n" << endl;
        for (int i = 0; i < altura * 2; i++) {
            // Nivel de nodos (líneas pares)
            if (i % 2 == 0) {
                for (int j = 0; j < ancho; j++) {
                    cout << niveles[i][j];
                }
                cout << endl;
            } 
            // Nivel de ramas (líneas impares)
            else {
                for (int j = 0; j < ancho; j++) {
                    if (tieneHijo[i][j]) {
                        cout << niveles[i][j];
                    } else {
                        cout << "   ";
                    }
                }
                cout << endl;
            }
        }
        cout << endl;
    }

    private:
    // Función para calcular la altura del árbol
    int calcularAltura(Nodo* nodo) {
        if (nodo == nullptr) return 0;
        return 1 + max(calcularAltura(nodo->izquierdo), calcularAltura(nodo->derecho));
    }
    
    // Función recursiva para llenar la matriz de visualización
    void llenarMatriz(Nodo* nodo, vector<vector<string>>& niveles, 
                    vector<vector<bool>>& tieneHijo, int nivel, int pos, 
                    int alturaTotal, int offset) {
        if (nodo == nullptr) return;
        
        int filaNodo = nivel * 2;
        int espacio = pow(2, alturaTotal - nivel - 1);
        
        // Guardar el valor del nodo
        string valorStr = " ";
        valorStr += nodo->dato;
        valorStr += " ";
        niveles[filaNodo][pos] = valorStr;
        
        // Conectar con hijos si existen
        if (nodo->izquierdo != nullptr) {
            int filaRama = filaNodo + 1;
            int posIzq = pos - espacio / 2;
            
            // Dibujar rama izquierda
            niveles[filaRama][posIzq] = "  /";
            tieneHijo[filaRama][posIzq] = true;
            
            // Conectar con diagonal
            for (int j = posIzq + 1; j < pos; j++) {
                niveles[filaRama][j] = "___";
                tieneHijo[filaRama][j] = true;
            }
            
            // Llamada recursiva para hijo izquierdo
            llenarMatriz(nodo->izquierdo, niveles, tieneHijo, nivel + 1, 
                        pos - espacio, alturaTotal, offset);
        }
        
        if (nodo->derecho != nullptr) {
            int filaRama = filaNodo + 1;
            int posDer = pos + espacio / 2;
            
            // Dibujar rama derecha
            niveles[filaRama][posDer] = "\\  ";
            tieneHijo[filaRama][posDer] = true;
            
            // Conectar con diagonal
            for (int j = pos + 1; j < posDer; j++) {
                niveles[filaRama][j] = "___";
                tieneHijo[filaRama][j] = true;
            }
            
            // Llamada recursiva para hijo derecho
            llenarMatriz(nodo->derecho, niveles, tieneHijo, nivel + 1, 
                        pos + espacio, alturaTotal, offset);
        }
    }
};

int main() {
    Arbol arbolito;
    //arbolito.insertar(5);   // 0   
    //arbolito.insertar(2);   // 1
    //arbolito.insertar(1);   // 1
    //arbolito.insertar(3);   // 1
    //arbolito.insertar(9);   // 1
    //arbolito.insertar(7);   // 2
    //arbolito.insertar(6);   // 2
    //arbolito.insertar(8);   // 2
    //arbolito.insertar(10);  // 2
    //arbolito.insertar(11);  // 3

    // usa el valor ASCII de las letras para hcaer comparacioes en los metods del aarbol
    arbolito.insertar('h');   
    arbolito.insertar('o');   
    arbolito.insertar('l');   
    arbolito.insertar('a');      
    arbolito.insertar(' ');   
    arbolito.insertar('m');   
    arbolito.insertar('u');   
    arbolito.insertar('n');   
    arbolito.insertar('d');   
    arbolito.insertar('o');   

    arbolito.imprimir(arbolito.raiz);
    cout << endl;
    arbolito.imprimirArbol(arbolito.raiz);
    cout << endl;
    arbolito.visualizarHorizontal();
    cout << endl;

    cout << "Pre-order: ";
    arbolito.preOrder(); 
    cout << endl;

    cout << "In-order: ";
    arbolito.inOrder();
    cout << endl;

    cout << "Post-order: ";
    arbolito.postOrder();
    cout << endl;

    arbolito.eliminar('m');
    cout << "\nDespues de eliminar 'm':\n";
    arbolito.imprimir(arbolito.raiz);
    cout << endl;
    arbolito.imprimirArbol(arbolito.raiz);
    cout << endl;
    arbolito.visualizarHorizontal();
    cout << endl;

    arbolito.preOrder();
    cout << endl;
    arbolito.inOrder();
    cout << endl;
    arbolito.postOrder();
    cout << endl;
    arbolito.buscar('a') ? cout << "\nEl nodo 'a' fue encontrado en el arbol.\n" 
                        : cout << "\nEl nodo 'a' no fue encontrado en el arbol.\n";

    
    return 0;
}

// TODO: insertar (char), eliminar, buscar, imprimir
// TODO: pre-order, in-order, post-order