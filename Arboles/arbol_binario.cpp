#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <queue>

using namespace std;

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
}

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
    Nodo* raiz;       // Puntero a la raiz del arbol

    // Constructor del arbol
    Arbol() {
        raiz = nullptr; // El arbol inicia vacio
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

        // Primero el subarbol derecho
        imprimir(nodo->derecho, nivel + 1);

        // Espacios segun el nivel
        for (int i = 0; i < nivel; i++)
            cout << "    ";

        // Imprimir el dato
        cout << nodo->dato << endl;

        // Luego el subarbol izquierdo
        imprimir(nodo->izquierdo, nivel + 1);
    }

    void visualizarHorizontal() {
        if (raiz == nullptr) {
            cout << "El arbol esta vacio." << endl;
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
        
        // Llenar la matriz con los valores del arbol
        llenarMatriz(raiz, niveles, tieneHijo, 0, ancho / 2, altura, 0);
        
        // Imprimir el arbol
        cout << "\nARBOL BINARIO (Formato Horizontal):\n" << endl;
        for (int i = 0; i < altura * 2; i++) {
            // Nivel de nodos (lineas pares)
            if (i % 2 == 0) {
                for (int j = 0; j < ancho; j++) {
                    cout << niveles[i][j];
                }
                cout << endl;
            } 
            // Nivel de ramas (lineas impares)
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
    // Funcion para calcular la altura del arbol
    int calcularAltura(Nodo* nodo) {
        if (nodo == nullptr) return 0;
        return 1 + max(calcularAltura(nodo->izquierdo), calcularAltura(nodo->derecho));
    }
    
    // Funcion recursiva para llenar la matriz de visualizacion
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
                niveles[filaRama][j] = "   ";
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
                niveles[filaRama][j] = "   ";
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
    int opcion;
    string cadena;
    char caracter;
    string cadenaActual = "";  // Para mostrar la cadena actual en el árbol

    do {
        limpiarPantalla();
        cout << "===========================================\n";
        cout << "        ARBOL BINARIO DE CADENAS\n";
        cout << "===========================================\n";
        
        // Mostrar la cadena actual almacenada
        if (!cadenaActual.empty()) {
            cout << "Cadena en el arbol: \"" << cadenaActual << "\"\n";
        } else {
            cout << "Arbol vacio\n";
        }
        cout << "===========================================\n";
        
        cout << "\n===== MENU PRINCIPAL =====\n";
        cout << "1. Insertar nueva cadena (reemplaza actual)\n";
        cout << "2. Agregar caracter individual\n";
        cout << "3. Agregar a la cadena actual\n";
        cout << "4. Eliminar caracter\n";
        cout << "5. Buscar caracter\n";
        cout << "6. Mostrar recorridos\n";
        cout << "7. Visualizar arbol (horizontal)\n";
        cout << "8. Imprimir arbol (vertical)\n";
        cout << "9. Vaciar arbol\n";
        cout << "0. Salir\n";
        cout << "\nSeleccione una opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar buffer

        switch (opcion) {
            case 1: {
                limpiarPantalla();
                cout << "===== INSERTAR NUEVA CADENA =====\n";
                cout << "Introduce una cadena de texto: ";
                getline(cin, cadena);
                
                // Vaciar el árbol y la cadena actual
                arbolito = Arbol();  // Crear nuevo árbol
                cadenaActual = "";
                
                // Insertar nueva cadena
                for (char c : cadena) {
                    arbolito.insertar(c);
                    cadenaActual += c;
                }
                
                cout << "\nCadena \"" << cadena << "\" insertada exitosamente.\n";
                pausar();
                break;
            }
                
            case 2: {
                limpiarPantalla();
                cout << "===== INSERTAR CARACTER INDIVIDUAL =====\n";
                cout << "Introduce un caracter: ";
                cin >> caracter;
                arbolito.insertar(caracter);
                cadenaActual += caracter;
                
                cout << "\nCaracter '" << caracter << "' insertado.\n";
                cout << "Cadena actual: \"" << cadenaActual << "\"\n";
                pausar();
                break;
            }
                
            case 3: {
                limpiarPantalla();
                cout << "===== AGREGAR A CADENA ACTUAL =====\n";
                cout << "Cadena actual: \"" << cadenaActual << "\"\n";
                cout << "Introduce texto para agregar: ";
                getline(cin, cadena);
                
                for (char c : cadena) {
                    arbolito.insertar(c);
                    cadenaActual += c;
                }
                
                cout << "\nTexto agregado exitosamente.\n";
                cout << "Nueva cadena: \"" << cadenaActual << "\"\n";
                pausar();
                break;
            }
                
            case 4: {
                limpiarPantalla();
                cout << "===== ELIMINAR CARACTER =====\n";
                if (cadenaActual.empty()) {
                    cout << "El arbol esta vacio.\n";
                } else {
                    cout << "Cadena actual: \"" << cadenaActual << "\"\n";
                    cout << "Introduce el caracter a eliminar: ";
                    cin >> caracter;
                    
                    // Verificar si el caracter existe
                    if (cadenaActual.find(caracter) != string::npos) {
                        arbolito.eliminar(caracter);
                        
                        // Actualizar la cadena (eliminar primera ocurrencia)
                        size_t pos = cadenaActual.find(caracter);
                        if (pos != string::npos) {
                            cadenaActual.erase(pos, 1);
                        }
                        
                        cout << "\nCaracter '" << caracter << "' eliminado.\n";
                    } else {
                        cout << "\n✗ El caracter '" << caracter << "' no existe en la cadena.\n";
                    }
                }
                pausar();
                break;
            }
                
            case 5: {
                limpiarPantalla();
                cout << "===== BUSCAR CARACTER =====\n";
                if (cadenaActual.empty()) {
                    cout << "El arbol esta vacio.\n";
                } else {
                    cout << "Cadena actual: \"" << cadenaActual << "\"\n";
                    cout << "Introduce el caracter a buscar: ";
                    cin >> caracter;
                    
                    if (arbolito.buscar(caracter)) {
                        cout << "\nEl caracter '" << caracter << "' SI esta en el arbol.\n";
                    } else {
                        cout << "\n✗ El caracter '" << caracter << "' NO esta en el arbol.\n";
                    }
                }
                pausar();
                break;
            }
                
            case 6: {
                limpiarPantalla();
                cout << "===== RECORRIDOS DEL ARBOL =====\n";
                if (cadenaActual.empty()) {
                    cout << "El arbol esta vacio.\n";
                } else {
                    cout << "Cadena: \"" << cadenaActual << "\"\n\n";
                    cout << "Pre-order:  ";
                    arbolito.preOrder();
                    cout << "\n\nIn-order:   ";
                    arbolito.inOrder();
                    cout << "\n\nPost-order: ";
                    arbolito.postOrder();
                    cout << endl;
                }
                pausar();
                break;
            }
                
            case 7: {
                limpiarPantalla();
                cout << "===== VISUALIZACION HORIZONTAL =====\n";
                if (cadenaActual.empty()) {
                    cout << "El arbol esta vacio.\n";
                } else {
                    cout << "Cadena: \"" << cadenaActual << "\"\n";
                    arbolito.visualizarHorizontal();
                }
                pausar();
                break;
            }
                
            case 8: {
                limpiarPantalla();
                cout << "===== VISUALIZACION VERTICAL =====\n";
                if (cadenaActual.empty()) {
                    cout << "El arbol esta vacio.\n";
                } else {
                    cout << "Cadena: \"" << cadenaActual << "\"\n";
                    cout << "\nArbol Binario (formato vertical):\n";
                    arbolito.imprimir(arbolito.raiz);
                }
                pausar();
                break;
            }
                
            case 9: {
                limpiarPantalla();
                cout << "===== VACIAR ARBOL =====\n";
                arbolito = Arbol();  // Crear nuevo árbol vacío
                cadenaActual = "";
                cout << "\nArbol vaciado exitosamente.\n";
                pausar();
                break;
            }
                
            case 0: {
                limpiarPantalla();
                cout << "===========================================\n";
                cout << "        SALIENDO DEL PROGRAMA\n";
                cout << "===========================================\n";
                cout << "\nGracias por usar el Arbol Binario de Cadenas\n";
                break;
            }
                
            default: {
                cout << "\n✗ Opcion no valida.\n";
                pausar();
                break;
            }
        }
    } while (opcion != 0);

    return 0;
}