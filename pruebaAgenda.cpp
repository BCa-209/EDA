#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

// ==================== CONSTANTES DE COLORES ====================
#ifdef _WIN32
    #include <windows.h>
    #define COLOR_RESET 7
    #define COLOR_ROJO 12
    #define COLOR_VERDE 10
    #define COLOR_AMARILLO 14
    #define COLOR_AZUL 11
    #define COLOR_MAGENTA 13
    #define COLOR_CYAN 9
    #define COLOR_BLANCO 15
    #define COLOR_GRIS 8
    
    void setColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }
#else
    // Códigos ANSI para Linux/Mac
    #define COLOR_RESET "\033[0m"
    #define COLOR_ROJO "\033[31m"
    #define COLOR_VERDE "\033[32m"
    #define COLOR_AMARILLO "\033[33m"
    #define COLOR_AZUL "\033[34m"
    #define COLOR_MAGENTA "\033[35m"
    #define COLOR_CYAN "\033[36m"
    #define COLOR_BLANCO "\033[37m"
    #define COLOR_GRIS "\033[90m"
    
    void setColor(const char* color) {
        cout << color;
    }
#endif

// ==================== FUNCIONES DE INTERFAZ ====================
void clean() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mostrarEncabezado(const string& titulo) {
    clean();
    
    #ifdef _WIN32
        setColor(COLOR_CYAN);
    #else
        setColor(COLOR_CYAN);
    #endif
    
    cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    cout << "║"; 
    
    #ifdef _WIN32
        setColor(COLOR_MAGENTA);
    #else
        setColor(COLOR_MAGENTA);
    #endif
    
    cout << "                    S I S T E M A   D E   A G E N D A                           ";
    
    #ifdef _WIN32
        setColor(COLOR_CYAN);
    #else
        setColor(COLOR_CYAN);
    #endif
    
    cout << "║\n";
    cout << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
    
    #ifdef _WIN32
        setColor(COLOR_AMARILLO);
    #else
        setColor(COLOR_AMARILLO);
    #endif
    
    cout << "║  " << left << setw(73) << titulo << " ║\n";
    
    #ifdef _WIN32
        setColor(COLOR_CYAN);
    #else
        setColor(COLOR_CYAN);
    #endif
    
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n\n";
    
    #ifdef _WIN32
        setColor(COLOR_RESET);
    #else
        setColor(COLOR_RESET);
    #endif
}

void mostrarMensaje(const string& mensaje, bool esError = false) {
    #ifdef _WIN32
        setColor(esError ? COLOR_ROJO : COLOR_VERDE);
    #else
        setColor(esError ? COLOR_ROJO : COLOR_VERDE);
    #endif
    
    if (esError) {
        cout << "✗ ";
    } else {
        cout << "✓ ";
    }
    
    cout << mensaje << endl;
    
    #ifdef _WIN32
        setColor(COLOR_RESET);
    #else
        setColor(COLOR_RESET);
    #endif
}

void mostrarOpcion(int num, const string& texto) {
    #ifdef _WIN32
        setColor(COLOR_AZUL);
    #else
        setColor(COLOR_AZUL);
    #endif
    
    cout << "  " << num << ". ";
    
    #ifdef _WIN32
        setColor(COLOR_BLANCO);
    #else
        setColor(COLOR_BLANCO);
    #endif
    
    cout << texto << endl;
}

void mostrarEntrada(const string& texto) {
    #ifdef _WIN32
        setColor(COLOR_AMARILLO);
    #else
        setColor(COLOR_AMARILLO);
    #endif
    
    cout << "  " << texto;
    
    #ifdef _WIN32
        setColor(COLOR_VERDE);
    #else
        setColor(COLOR_VERDE);
    #endif
    
    cout << " ➤ ";
    
    #ifdef _WIN32
        setColor(COLOR_RESET);
    #else
        setColor(COLOR_RESET);
    #endif
}

void mostrarLineaSeparadora() {
    #ifdef _WIN32
        setColor(COLOR_GRIS);
    #else
        setColor(COLOR_GRIS);
    #endif
    
    cout << "\n────────────────────────────────────────────────────────────────────────────────\n\n";
    
    #ifdef _WIN32
        setColor(COLOR_RESET);
    #else
        setColor(COLOR_RESET);
    #endif
}

void mostrarSubtitulo(const string& texto) {
    #ifdef _WIN32
        setColor(COLOR_MAGENTA);
    #else
        setColor(COLOR_MAGENTA);
    #endif
    
    cout << "\n◆ " << texto << "\n";
    
    #ifdef _WIN32
        setColor(COLOR_GRIS);
    #else
        setColor(COLOR_GRIS);
    #endif
    
    cout << "  " << string(texto.length() + 2, '─') << "\n\n";
    
    #ifdef _WIN32
        setColor(COLOR_RESET);
    #else
        setColor(COLOR_RESET);
    #endif
}

// ==================== CLASE NODO (SIN CAMBIOS) ====================
class Nodo {
public:
    static int contador_id;

    int id;
    string nombres;
    string apellidoP;
    string apellidoM;
    string telefonos;
    string correo;
    string direccion;
    string padre_id;
    string madre_id;
    
    Nodo* anterior;
    Nodo* siguiente;

    Nodo(int identificador, string nom, string apP, string apM, string tel, string mail, string dir, string p_id, string m_id) {
        id = identificador;
        nombres = nom;
        apellidoP = apP;
        apellidoM = apM;
        telefonos = tel;
        correo = mail;
        direccion = dir;
        padre_id = p_id;
        madre_id = m_id;
        
        anterior = nullptr;
        siguiente = nullptr;
    }
};

int Nodo::contador_id = 1;

// ==================== CLASE LISTA (SIN CAMBIOS) ====================
class Lista {
private:
    Nodo* cabezaptr;

public:
    Lista() {
        cabezaptr = nullptr;
    }

    void addIni(string nom, string apP, string apM, string tel, string mail, string dir, string p_id, string m_id) {
        Nodo* nuevo = new Nodo(Nodo::contador_id++, nom, apP, apM, tel, mail, dir, p_id, m_id);
        nuevo->siguiente = cabezaptr;
        if(cabezaptr != nullptr) {
            cabezaptr->anterior = nuevo;
        }
        cabezaptr = nuevo;
    }

    void add(string nom, string apP, string apM, string tel, string mail, string dir, string p_id, string m_id) {
        Nodo* nuevo = new Nodo(Nodo::contador_id++, nom, apP, apM, tel, mail, dir, p_id, m_id);

        if (cabezaptr == nullptr) {
            cabezaptr = nuevo;
            return;
        }

        Nodo* temp = cabezaptr;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }

        temp->siguiente = nuevo;
        nuevo->anterior = temp;
    }

    void addPos(int pos, string nom, string apP, string apM, string tel, string mail, string dir, string p_id, string m_id) {
        if (pos < 0) return;
        if (pos == 0) {
            addIni(nom, apP, apM, tel, mail, dir, p_id, m_id);
            return;
        }
        
        Nodo* aux = cabezaptr;
        for (int i = 0; i < pos - 1; i++) {
            if (aux == nullptr) return;
            aux = aux->siguiente;
        }
        if (aux == nullptr) return;
        
        Nodo* nuevo = new Nodo(Nodo::contador_id++, nom, apP, apM, tel, mail, dir, p_id, m_id);
        Nodo* siguienteNodo = aux->siguiente;
        
        nuevo->siguiente = siguienteNodo;
        nuevo->anterior = aux;
        aux->siguiente = nuevo;
        
        if(siguienteNodo != nullptr) {
            siguienteNodo->anterior = nuevo;
        }
    }

    void dropIni() {
        if (cabezaptr == nullptr) return;
        Nodo* temp = cabezaptr;
        cabezaptr = cabezaptr->siguiente;
        
        if (cabezaptr != nullptr) {
            cabezaptr->anterior = nullptr;
        }
        
        delete temp;
    }
    
    void drop() {
        if (cabezaptr == nullptr) return;
        
        if (cabezaptr->siguiente == nullptr) {
            delete cabezaptr;
            cabezaptr = nullptr;
            return;
        }
        
        Nodo* temp = cabezaptr;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        
        temp->anterior->siguiente = nullptr;
        delete temp;
    }
    
    void dropPos(int pos) {
        if (cabezaptr == nullptr || pos < 0) return;
        if (pos == 0) {
            dropIni();
            return;
        }
        
        Nodo* actual = cabezaptr;
        for (int i=0; i<pos; i++) {
            if(actual == nullptr) return;
            actual = actual->siguiente;
        }
        
        if(actual == nullptr) return;
        
        Nodo* nodoAnterior = actual->anterior;
        Nodo* nodoSiguiente = actual->siguiente;
        
        if (nodoAnterior != nullptr) {
            nodoAnterior->siguiente = nodoSiguiente;
        }
        
        if (nodoSiguiente != nullptr) {
            nodoSiguiente->anterior = nodoAnterior;
        }
        
        delete actual;
    }
    
    Nodo* buscarPorID(int idBuscado) {
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            if (actual->id == idBuscado) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    vector<Nodo*> buscarPorNombre(string nombreBuscado) {
        vector<Nodo*> resultados;
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            if (actual->nombres.find(nombreBuscado) != string::npos) {
                resultados.push_back(actual);
            }
            actual = actual->siguiente;
        }
        return resultados;
    }
    
    Nodo* getCabeza() {
        return cabezaptr;
    }
    
    int getCantidad() {
        int count = 0;
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            count++;
            actual = actual->siguiente;
        }
        return count;
    }
    
    void print() {
        Nodo* actual = cabezaptr;
        
        if (actual == nullptr) {
            #ifdef _WIN32
                setColor(COLOR_AMARILLO);
            #else
                setColor(COLOR_AMARILLO);
            #endif
            cout << "╔══════════════════════════════════════════════════════════════════╗\n";
            cout << "║                    LA AGENDA ESTÁ VACÍA                          ║\n";
            cout << "╚══════════════════════════════════════════════════════════════════╝\n";
            #ifdef _WIN32
                setColor(COLOR_RESET);
            #else
                setColor(COLOR_RESET);
            #endif
            return;
        }
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "┌──────────────────────────────────────────────────────────────────────┐\n";
        cout << "│                     CONTACTOS EN LA AGENDA                          │\n";
        cout << "├──────────────────────────────────────────────────────────────────────┤\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        int contador = 0;
        while (actual != nullptr) {
            contador++;
            
            #ifdef _WIN32
                setColor(COLOR_MAGENTA);
            #else
                setColor(COLOR_MAGENTA);
            #endif
            cout << "├── Contacto #" << contador << " ";
            cout << string(60, '─') << "┤\n";
            
            #ifdef _WIN32
                setColor(COLOR_AZUL);
            #else
                setColor(COLOR_AZUL);
            #endif
            cout << "│  ID: ";
            #ifdef _WIN32
                setColor(COLOR_VERDE);
            #else
                setColor(COLOR_VERDE);
            #endif
            cout << actual->id << "\n";
            
            #ifdef _WIN32
                setColor(COLOR_AZUL);
            #else
                setColor(COLOR_AZUL);
            #endif
            cout << "│  Nombre: ";
            #ifdef _WIN32
                setColor(COLOR_BLANCO);
            #else
                setColor(COLOR_BLANCO);
            #endif
            cout << actual->nombres << " " << actual->apellidoP << " " << actual->apellidoM << "\n";
            
            #ifdef _WIN32
                setColor(COLOR_AZUL);
            #else
                setColor(COLOR_AZUL);
            #endif
            cout << "│  Teléfono: ";
            #ifdef _WIN32
                setColor(COLOR_BLANCO);
            #else
                setColor(COLOR_BLANCO);
            #endif
            cout << actual->telefonos << "\n";
            
            #ifdef _WIN32
                setColor(COLOR_AZUL);
            #else
                setColor(COLOR_AZUL);
            #endif
            cout << "│  Correo: ";
            #ifdef _WIN32
                setColor(COLOR_BLANCO);
            #else
                setColor(COLOR_BLANCO);
            #endif
            cout << actual->correo << "\n";
            
            #ifdef _WIN32
                setColor(COLOR_AZUL);
            #else
                setColor(COLOR_AZUL);
            #endif
            cout << "│  Dirección: ";
            #ifdef _WIN32
                setColor(COLOR_BLANCO);
            #else
                setColor(COLOR_BLANCO);
            #endif
            cout << actual->direccion << "\n";
            
            #ifdef _WIN32
                setColor(COLOR_AZUL);
            #else
                setColor(COLOR_AZUL);
            #endif
            cout << "│  Familia: ";
            #ifdef _WIN32
                setColor(COLOR_AMARILLO);
            #else
                setColor(COLOR_AMARILLO);
            #endif
            cout << "Padre ID: " << (actual->padre_id.empty() ? "N/A" : actual->padre_id);
            cout << " │ Madre ID: " << (actual->madre_id.empty() ? "N/A" : actual->madre_id) << "\n";
            
            actual = actual->siguiente;
        }
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "└──────────────────────────────────────────────────────────────────────┘\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        #ifdef _WIN32
            setColor(COLOR_VERDE);
        #else
            setColor(COLOR_VERDE);
        #endif
        cout << "\nTotal de contactos mostrados: " << contador << endl;
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
    }
    
    ~Lista() {
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
};

// ==================== CLASE AGENDA ====================
class Agenda {
private:
    string nombreArchivo;
    Lista* listaContactos;
    
public:
    Agenda() {
        nombreArchivo = "agenda.txt";
        listaContactos = new Lista();
    }
    
    bool existeID(string idStr) {
        if (idStr.empty()) return true;
        try {
            int id = stoi(idStr);
            return listaContactos->buscarPorID(id) != nullptr;
        } catch (...) {
            return false;
        }
    }
    
    void guardarEnArchivo() {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            mostrarMensaje("Error al abrir el archivo para guardar.", true);
            return;
        }

        Nodo* actual = listaContactos->getCabeza();
        while (actual != nullptr) {
            archivo << "BEGIN:VCARD\n";
            archivo << "VERSION:2.1\n";
            archivo << "ID:" << actual->id << "\n";
            archivo << "N:" << actual->apellidoP << ";"
                            << actual->nombres << ";"
                            << actual->apellidoM << ";;;\n";
            archivo << "FN:" << actual->nombres << " "
                << actual->apellidoP << " "
                << actual->apellidoM << "\n";

            stringstream ss(actual->telefonos);
            string tel;
            while (getline(ss, tel, ';')) {
                if (!tel.empty())
                    archivo << "TEL;CELL:" << tel << "\n";
            }

            archivo << "EMAIL:" << actual->correo << "\n";
            archivo << "ADR:" << actual->direccion << "\n";
            archivo << "PADRE_ID:" << actual->padre_id << "\n";
            archivo << "MADRE_ID:" << actual->madre_id << "\n";
            archivo << "END:VCARD\n\n";

            actual = actual->siguiente;
        }

        archivo.close();
        mostrarMensaje("Agenda guardada correctamente en formato vCard TXT");
    }

    void cargarDesdeArchivo() {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            mostrarMensaje("No se encontró archivo de agenda existente. Se creará uno nuevo.", false);
            return;
        }

        while (listaContactos->getCabeza() != nullptr) {
            listaContactos->dropIni();
        }

        Nodo::contador_id = 1;

        string linea;
        int id = 0;
        string nombres, apellidoP, apellidoM, telefonos, correo, direccion;
        string padre_id, madre_id;

        while (getline(archivo, linea)) {
            if (linea == "BEGIN:VCARD") {
                id = 0;
                nombres = apellidoP = apellidoM = telefonos = "";
                correo = direccion = "";
                padre_id = madre_id = "";
            }
            else if (linea.rfind("ID:", 0) == 0) {
                id = stoi(linea.substr(3));
            }
            else if (linea.rfind("N:", 0) == 0) {
                string datos = linea.substr(2);
                stringstream ss(datos);
                getline(ss, apellidoP, ';');
                getline(ss, nombres, ';');
                getline(ss, apellidoM, ';');
            }
            else if (linea.rfind("TEL", 0) == 0) {
                string tel = linea.substr(linea.find(':') + 1);
                if (telefonos.empty())
                    telefonos = tel;
                else
                    telefonos += ";" + tel;
            }
            else if (linea.rfind("EMAIL:", 0) == 0) {
                correo = linea.substr(6);
            }
            else if (linea.rfind("ADR:", 0) == 0) {
                direccion = linea.substr(4);
            }
            else if (linea.rfind("PADRE_ID:", 0) == 0) {
                padre_id = linea.substr(9);
            }
            else if (linea.rfind("MADRE_ID:", 0) == 0) {
                madre_id = linea.substr(9);
            }
            else if (linea == "END:VCARD") {
                listaContactos->add(
                    nombres,
                    apellidoP,
                    apellidoM,
                    telefonos,
                    correo,
                    direccion,
                    padre_id,
                    madre_id
                );

                if (id >= Nodo::contador_id)
                    Nodo::contador_id = id + 1;
            }
        }

        archivo.close();
        mostrarMensaje("Agenda cargada correctamente desde vCard TXT");
    }

    void agregarContacto(string nom, string apP, string apM, string tel, string mail, string dir, string p_id = "", string m_id = "") {
        listaContactos->add(nom, apP, apM, tel, mail, dir, p_id, m_id);
        mostrarMensaje("Contacto agregado correctamente.");
    }
    
    void mostrarListaRapida() {
        Nodo* actual = listaContactos->getCabeza();
        if (actual == nullptr) {
            #ifdef _WIN32
                setColor(COLOR_AMARILLO);
            #else
                setColor(COLOR_AMARILLO);
            #endif
            cout << "  [ Agenda vacía - No hay contactos disponibles ]\n";
            #ifdef _WIN32
                setColor(COLOR_RESET);
            #else
                setColor(COLOR_RESET);
            #endif
            return;
        }
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "\n┌──────────────────────────────────────────────────────────┐\n";
        cout << "│          CONTACTOS DISPONIBLES (ID y Nombre)           │\n";
        cout << "├──────────────────────────────────────────────────────────┤\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        int cont = 1;
        while (actual != nullptr) {
            #ifdef _WIN32
                setColor(COLOR_AZUL);
            #else
                setColor(COLOR_AZUL);
            #endif
            cout << "│  ";
            #ifdef _WIN32
                setColor(COLOR_VERDE);
            #else
                setColor(COLOR_VERDE);
            #endif
            cout << "ID: " << setw(3) << left << actual->id;
            #ifdef _WIN32
                setColor(COLOR_BLANCO);
            #else
                setColor(COLOR_BLANCO);
            #endif
            cout << " → " << actual->nombres << " " << actual->apellidoP;
            if (cont < 10) cout << " ";
            cout << " │\n";
            actual = actual->siguiente;
            cont++;
        }
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "└──────────────────────────────────────────────────────────┘\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
    }

    void editarContactoPorID(int id) {
        Nodo* actual = listaContactos->buscarPorID(id);
        if (actual == nullptr) {
            mostrarMensaje("No se encontró contacto con ID " + to_string(id), true);
            return;
        }

        mostrarSubtitulo("EDITANDO CONTACTO ID: " + to_string(id));
        
        #ifdef _WIN32
            setColor(COLOR_AMARILLO);
        #else
            setColor(COLOR_AMARILLO);
        #endif
        cout << "  (Deja vacío y presiona ENTER para mantener el valor actual)\n\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif

        string input;
        
        mostrarEntrada("Nombre [" + actual->nombres + "]: ");
        getline(cin, input);
        if (!input.empty()) actual->nombres = input;

        mostrarEntrada("Apellido Paterno [" + actual->apellidoP + "]: ");
        getline(cin, input);
        if (!input.empty()) actual->apellidoP = input;
        
        mostrarEntrada("Apellido Materno [" + actual->apellidoM + "]: ");
        getline(cin, input);
        if (!input.empty()) actual->apellidoM = input;

        mostrarEntrada("Teléfonos [" + actual->telefonos + "]: ");
        getline(cin, input);
        if (!input.empty()) actual->telefonos = input;

        mostrarEntrada("Correo [" + actual->correo + "]: ");
        getline(cin, input);
        if (!input.empty()) actual->correo = input;

        mostrarEntrada("Dirección [" + actual->direccion + "]: ");
        getline(cin, input);
        if (!input.empty()) actual->direccion = input;

        mostrarListaRapida();

        // Validación para Padre
        do {
            mostrarEntrada("Padre ID [" + actual->padre_id + "]: ");
            getline(cin, input);
            
            if (input.empty()) break;

            if (input == to_string(id)) {
                mostrarMensaje("Error: Un contacto no puede ser su propio padre.", true);
            } else if (!existeID(input)) {
                mostrarMensaje("Error: El ID " + input + " no existe.", true);
            } else {
                actual->padre_id = input;
                break;
            }
        } while (true);

        // Validación para Madre
        do {
            mostrarEntrada("Madre ID [" + actual->madre_id + "]: ");
            getline(cin, input);

            if (input.empty()) break;

            if (input == to_string(id)) {
                mostrarMensaje("Error: Un contacto no puede ser su propia madre.", true);
            } else if (!existeID(input)) {
                mostrarMensaje("Error: El ID " + input + " no existe.", true);
            } else {
                actual->madre_id = input;
                break;
            }
        } while (true);

        mostrarMensaje("Contacto actualizado correctamente");
    }

    void eliminarContactoPorID(int id) {
        Nodo* actual = listaContactos->getCabeza();
        int pos = 0;
        while (actual != nullptr) {
            if (actual->id == id) {
                #ifdef _WIN32
                    setColor(COLOR_ROJO);
                #else
                    setColor(COLOR_ROJO);
                #endif
                cout << "\n⚠ ¿Está seguro de eliminar el contacto ID " << id << "? (s/n): ";
                #ifdef _WIN32
                    setColor(COLOR_RESET);
                #else
                    setColor(COLOR_RESET);
                #endif
                
                char confirmar;
                cin >> confirmar;
                cin.ignore();
                
                if (confirmar == 's' || confirmar == 'S') {
                    listaContactos->dropPos(pos);
                    mostrarMensaje("Contacto con ID " + to_string(id) + " eliminado permanentemente.");
                } else {
                    mostrarMensaje("Eliminación cancelada.");
                }
                return;
            }
            actual = actual->siguiente;
            pos++;
        }
        mostrarMensaje("No se encontró contacto con ID " + to_string(id), true);
    }
    
    void buscarContacto(string nombre) {
        vector<Nodo*> resultados = listaContactos->buscarPorNombre(nombre);
        
        if (resultados.empty()) {
            mostrarMensaje("No se encontraron contactos con el nombre: " + nombre, true);
            return;
        }
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "\n┌──────────────────────────────────────────────────────────┐\n";
        cout << "│        RESULTADOS DE BÚSQUEDA PARA: " << left << setw(20) << nombre << "       │\n";
        cout << "├──────────────────────────────────────────────────────────┤\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        for (size_t i = 0; i < resultados.size(); i++) {
            Nodo* nodo = resultados[i];
            
            #ifdef _WIN32
                setColor(COLOR_VERDE);
            #else
                setColor(COLOR_VERDE);
            #endif
            cout << "│  ID: " << nodo->id;
            #ifdef _WIN32
                setColor(COLOR_BLANCO);
            #else
                setColor(COLOR_BLANCO);
            #endif
            cout << " | " << nodo->nombres << " " << nodo->apellidoP;
            
            int espacios = 40 - (nodo->nombres.length() + nodo->apellidoP.length());
            cout << string(max(0, espacios), ' ') << "│\n";
            
            #ifdef _WIN32
                setColor(COLOR_GRIS);
            #else
                setColor(COLOR_GRIS);
            #endif
            cout << "│  📞 " << nodo->telefonos;
            espacios = 47 - nodo->telefonos.length();
            cout << string(max(0, espacios), ' ') << "│\n";
            
            #ifdef _WIN32
                setColor(COLOR_GRIS);
            #else
                setColor(COLOR_GRIS);
            #endif
            cout << "│  ✉ " << nodo->correo;
            espacios = 47 - nodo->correo.length();
            cout << string(max(0, espacios), ' ') << "│\n";
            
            if (i < resultados.size() - 1) {
                #ifdef _WIN32
                    setColor(COLOR_CYAN);
                #else
                    setColor(COLOR_CYAN);
                #endif
                cout << "├──────────────────────────────────────────────────────────┤\n";
                #ifdef _WIN32
                    setColor(COLOR_RESET);
                #else
                    setColor(COLOR_RESET);
                #endif
            }
        }
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "└──────────────────────────────────────────────────────────┘\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        #ifdef _WIN32
            setColor(COLOR_VERDE);
        #else
            setColor(COLOR_VERDE);
        #endif
        cout << "\nSe encontraron " << resultados.size() << " contacto(s)\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
    }
    
    void mostrarAgenda() {
        listaContactos->print();
    }
    
    void mostrarEstadisticas() {
        int total = listaContactos->getCantidad();
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "\n┌──────────────────────────────────────────────────────────┐\n";
        cout << "│               ESTADÍSTICAS DE LA AGENDA                 │\n";
        cout << "├──────────────────────────────────────────────────────────┤\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        #ifdef _WIN32
            setColor(COLOR_AZUL);
        #else
            setColor(COLOR_AZUL);
        #endif
        cout << "│  ";
        #ifdef _WIN32
            setColor(COLOR_BLANCO);
        #else
            setColor(COLOR_BLANCO);
        #endif
        cout << "📊 Total de contactos: ";
        #ifdef _WIN32
            setColor(COLOR_VERDE);
        #else
            setColor(COLOR_VERDE);
        #endif
        cout << setw(4) << right << total << "                     │\n";
        
        #ifdef _WIN32
            setColor(COLOR_AZUL);
        #else
            setColor(COLOR_AZUL);
        #endif
        cout << "│  ";
        #ifdef _WIN32
            setColor(COLOR_BLANCO);
        #else
            setColor(COLOR_BLANCO);
        #endif
        cout << "🆔 Próximo ID disponible: ";
        #ifdef _WIN32
            setColor(COLOR_VERDE);
        #else
            setColor(COLOR_VERDE);
        #endif
        cout << setw(4) << right << Nodo::contador_id << "                     │\n";
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "└──────────────────────────────────────────────────────────┘\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
    }
    
    void mostrarRelacionesFamiliares(int id) {
        Nodo* persona = listaContactos->buscarPorID(id);
        if (persona == nullptr) {
            mostrarMensaje("Contacto con ID " + to_string(id) + " no encontrado.", true);
            return;
        }

        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "\n┌──────────────────────────────────────────────────────────┐\n";
        cout << "│            ÁRBOL FAMILIAR                                │\n";
        cout << "├──────────────────────────────────────────────────────────┤\n";
        #ifdef _WIN32
            setColor(COLOR_MAGENTA);
        #else
            setColor(COLOR_MAGENTA);
        #endif
        cout << "│  👤 ";
        #ifdef _WIN32
            setColor(COLOR_BLANCO);
        #else
            setColor(COLOR_BLANCO);
        #endif
        cout << persona->nombres << " " << persona->apellidoP;
        cout << " (ID: " << persona->id << ")                  │\n";
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "├──────────────────────────────────────────────────────────┤\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif

        // PADRES
        #ifdef _WIN32
            setColor(COLOR_AZUL);
        #else
            setColor(COLOR_AZUL);
        #endif
        cout << "│  👨‍👩‍👧 PADRES:                                            │\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        if (!persona->padre_id.empty()) {
            Nodo* p = listaContactos->buscarPorID(stoi(persona->padre_id));
            cout << "│    👨 Padre: ";
            #ifdef _WIN32
                setColor(p ? COLOR_VERDE : COLOR_AMARILLO);
            #else
                setColor(p ? COLOR_VERDE : COLOR_AMARILLO);
            #endif
            cout << (p ? p->nombres + " " + p->apellidoP + " (ID: " + persona->padre_id + ")" : 
                         "ID " + persona->padre_id + " [No en agenda]");
            #ifdef _WIN32
                setColor(COLOR_RESET);
            #else
                setColor(COLOR_RESET);
            #endif
            cout << "         │\n";
        } else {
            cout << "│    👨 Padre: ";
            #ifdef _WIN32
                setColor(COLOR_GRIS);
            #else
                setColor(COLOR_GRIS);
            #endif
            cout << "No registrado";
            #ifdef _WIN32
                setColor(COLOR_RESET);
            #else
                setColor(COLOR_RESET);
            #endif
            cout << "                              │\n";
        }

        if (!persona->madre_id.empty()) {
            Nodo* m = listaContactos->buscarPorID(stoi(persona->madre_id));
            cout << "│    👩 Madre: ";
            #ifdef _WIN32
                setColor(m ? COLOR_VERDE : COLOR_AMARILLO);
            #else
                setColor(m ? COLOR_VERDE : COLOR_AMARILLO);
            #endif
            cout << (m ? m->nombres + " " + m->apellidoP + " (ID: " + persona->madre_id + ")" : 
                         "ID " + persona->madre_id + " [No en agenda]");
            #ifdef _WIN32
                setColor(COLOR_RESET);
            #else
                setColor(COLOR_RESET);
            #endif
            cout << "         │\n";
        } else {
            cout << "│    👩 Madre: ";
            #ifdef _WIN32
                setColor(COLOR_GRIS);
            #else
                setColor(COLOR_GRIS);
            #endif
            cout << "No registrada";
            #ifdef _WIN32
                setColor(COLOR_RESET);
            #else
                setColor(COLOR_RESET);
            #endif
            cout << "                             │\n";
        }

        // HIJOS
        #ifdef _WIN32
            setColor(COLOR_AZUL);
        #else
            setColor(COLOR_AZUL);
        #endif
        cout << "│                                                          │\n";
        cout << "│  👶 HIJOS:                                              │\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        bool tieneHijos = false;
        Nodo* actual = listaContactos->getCabeza();
        string idBuscado = to_string(id);

        while (actual != nullptr) {
            if (actual->padre_id == idBuscado || actual->madre_id == idBuscado) {
                cout << "│    👤 ";
                #ifdef _WIN32
                    setColor(COLOR_VERDE);
                #else
                    setColor(COLOR_VERDE);
                #endif
                cout << actual->nombres << " " << actual->apellidoP;
                cout << " (ID: " << actual->id << ")";
                #ifdef _WIN32
                    setColor(COLOR_RESET);
                #else
                    setColor(COLOR_RESET);
                #endif
                
                int espacios = 40 - (actual->nombres.length() + actual->apellidoP.length() + to_string(actual->id).length());
                cout << string(max(0, espacios), ' ') << "│\n";
                tieneHijos = true;
            }
            actual = actual->siguiente;
        }

        if (!tieneHijos) {
            cout << "│    ";
            #ifdef _WIN32
                setColor(COLOR_GRIS);
            #else
                setColor(COLOR_GRIS);
            #endif
            cout << "(No se encontraron hijos registrados)";
            #ifdef _WIN32
                setColor(COLOR_RESET);
            #else
                setColor(COLOR_RESET);
            #endif
            cout << "           │\n";
        }

        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "└──────────────────────────────────────────────────────────┘\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
    }

    void exportarVCF() {
        string nombreVCF = "contactos_exportados.vcf";
        ofstream archivo(nombreVCF);
        
        if (!archivo.is_open()) {
            mostrarMensaje("Error al crear el archivo VCF.", true);
            return;
        }

        Nodo* actual = listaContactos->getCabeza();
        int contador = 0;
        while (actual != nullptr) {
            archivo << "BEGIN:VCARD\n";
            archivo << "VERSION:2.1\n";
            archivo << "N:" << actual->apellidoP << ";" 
                    << actual->nombres << ";" 
                    << actual->apellidoM << ";;;\n";
            archivo << "FN:" << actual->nombres << " " 
                    << actual->apellidoP << "\n";

            stringstream ss(actual->telefonos);
            string tel;
            while (getline(ss, tel, ';')) {
                if (!tel.empty()) {
                    archivo << "TEL;CELL:" << tel << "\n";
                }
            }

            archivo << "EMAIL:" << actual->correo << "\n";
            archivo << "ADR:" << actual->direccion << "\n";
            archivo << "END:VCARD\n";
            
            actual = actual->siguiente;
            contador++;
        }

        archivo.close();
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "\n┌──────────────────────────────────────────────────────────┐\n";
        cout << "│                 EXPORTACIÓN COMPLETADA                   │\n";
        cout << "├──────────────────────────────────────────────────────────┤\n";
        #ifdef _WIN32
            setColor(COLOR_VERDE);
        #else
            setColor(COLOR_VERDE);
        #endif
        cout << "│  Archivo: " << left << setw(44) << nombreVCF << " │\n";
        cout << "│  Contactos exportados: " << setw(33) << contador << " │\n";
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "└──────────────────────────────────────────────────────────┘\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        #ifdef _WIN32
            setColor(COLOR_AMARILLO);
        #else
            setColor(COLOR_AMARILLO);
        #endif
        cout << "\n💡 Puedes importar este archivo directamente en tu celular.\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
    }

    ~Agenda() {
        delete listaContactos;
    }
};

// ==================== MAIN MEJORADO ====================
int main() {
    Agenda miAgenda;
    int opcion;

    // Cargar datos existentes al iniciar
    mostrarEncabezado("CARGANDO AGENDA...");
    miAgenda.cargarDesdeArchivo();
    
    #ifdef _WIN32
        Sleep(1000);
    #else
        usleep(1000000);
    #endif

    while (true) {
        mostrarEncabezado("MENÚ PRINCIPAL");
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "┌──────────────────────────────────────────────────────────┐\n";
        cout << "│                     MENÚ DE OPCIONES                     │\n";
        cout << "├──────────────────────────────────────────────────────────┤\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        mostrarOpcion(1, "📝 Agregar nuevo contacto");
        mostrarOpcion(2, "👥 Mostrar todos los contactos");
        mostrarOpcion(3, "🔍 Buscar contacto por nombre");
        mostrarOpcion(4, "✏️  Editar contacto por ID");
        mostrarOpcion(5, "🗑️  Eliminar contacto por ID");
        mostrarOpcion(6, "💾 Guardar agenda en archivo");
        mostrarOpcion(7, "📊 Mostrar estadísticas");
        mostrarOpcion(8, "👨‍👩‍👧‍👦 Mostrar relaciones familiares");
        mostrarOpcion(9, "📤 Exportar a formato .VCF (Celulares)");
        mostrarOpcion(0, "🚪 Salir y guardar automáticamente");
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "└──────────────────────────────────────────────────────────┘\n\n";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        mostrarEntrada("Seleccione una opción");
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                mostrarEncabezado("AGREGAR NUEVO CONTACTO");
                string nom, apP, apM, tel, mail, dir, p_id, m_id;

                mostrarSubtitulo("Información Personal");
                mostrarEntrada("Nombres");
                getline(cin, nom);
                mostrarEntrada("Apellido Paterno");
                getline(cin, apP);
                mostrarEntrada("Apellido Materno");
                getline(cin, apM);
                mostrarEntrada("Teléfonos (separados por ;)");
                getline(cin, tel);
                mostrarEntrada("Correo electrónico");
                getline(cin, mail);
                mostrarEntrada("Dirección");
                getline(cin, dir);

                mostrarSubtitulo("Relaciones Familiares (Opcional)");
                miAgenda.mostrarListaRapida();
                
                do {
                    mostrarEntrada("ID del Padre (dejar vacío si no aplica)");
                    getline(cin, p_id);
                    if (p_id.empty() || miAgenda.existeID(p_id)) {
                        break;
                    }
                    mostrarMensaje("Error: El ID " + p_id + " no existe.", true);
                } while (true);
                
                do {
                    mostrarEntrada("ID de la Madre (dejar vacío si no aplica)");
                    getline(cin, m_id);
                    if (m_id.empty() || miAgenda.existeID(m_id)) {
                        break;
                    }
                    mostrarMensaje("Error: El ID " + m_id + " no existe.", true);
                } while (true);

                miAgenda.agregarContacto(nom, apP, apM, tel, mail, dir, p_id, m_id);
                break;
            }

            case 2:
                mostrarEncabezado("LISTA DE CONTACTOS");
                miAgenda.mostrarAgenda();
                break;

            case 3: {
                mostrarEncabezado("BUSCAR CONTACTO");
                string nombre;
                mostrarEntrada("Ingrese nombre o parte del nombre a buscar");
                getline(cin, nombre);
                miAgenda.buscarContacto(nombre);
                break;
            }

            case 4: {
                mostrarEncabezado("EDITAR CONTACTO");
                miAgenda.mostrarListaRapida();
                int id;
                mostrarEntrada("Ingrese ID del contacto a editar");
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                miAgenda.editarContactoPorID(id);
                break;
            }

            case 5: {
                mostrarEncabezado("ELIMINAR CONTACTO");
                miAgenda.mostrarListaRapida();
                int id;
                mostrarEntrada("Ingrese ID del contacto a eliminar");
                cin >> id;
                cin.ignore();
                miAgenda.eliminarContactoPorID(id);
                break;
            }

            case 6:
                mostrarEncabezado("GUARDAR AGENDA");
                miAgenda.guardarEnArchivo();
                break;

            case 7:
                mostrarEncabezado("ESTADÍSTICAS");
                miAgenda.mostrarEstadisticas();
                break;

            case 8: {
                mostrarEncabezado("RELACIONES FAMILIARES");
                miAgenda.mostrarListaRapida();
                int id;
                mostrarEntrada("Ingrese el ID del contacto para ver su familia");
                cin >> id;
                cin.ignore();
                miAgenda.mostrarRelacionesFamiliares(id);
                break;
            }

            case 9:
                mostrarEncabezado("EXPORTAR A VCF");
                miAgenda.exportarVCF();
                break;
                
            case 0:
                mostrarEncabezado("SALIENDO DEL SISTEMA");
                
                #ifdef _WIN32
                    setColor(COLOR_CYAN);
                #else
                    setColor(COLOR_CYAN);
                #endif
                cout << "\n┌──────────────────────────────────────────────────────────┐\n";
                cout << "│                GUARDANDO AUTOMÁTICAMENTE                 │\n";
                cout << "├──────────────────────────────────────────────────────────┤\n";
                #ifdef _WIN32
                    setColor(COLOR_VERDE);
                #else
                    setColor(COLOR_VERDE);
                #endif
                cout << "│  ⏳ Por favor espere...                                 │\n";
                #ifdef _WIN32
                    setColor(COLOR_CYAN);
                #else
                    setColor(COLOR_CYAN);
                #endif
                cout << "└──────────────────────────────────────────────────────────┘\n\n";
                #ifdef _WIN32
                    setColor(COLOR_RESET);
                #else
                    setColor(COLOR_RESET);
                #endif
                
                miAgenda.guardarEnArchivo();
                
                #ifdef _WIN32
                    Sleep(1500);
                #else
                    usleep(1500000);
                #endif
                
                #ifdef _WIN32
                    setColor(COLOR_MAGENTA);
                #else
                    setColor(COLOR_MAGENTA);
                #endif
                cout << "\n✨ ¡Gracias por usar el Sistema de Agenda! ✨\n\n";
                #ifdef _WIN32
                    setColor(COLOR_RESET);
                #else
                    setColor(COLOR_RESET);
                #endif
                
                return 0;

            default:
                mostrarMensaje("Opción inválida. Intente nuevamente.", true);
                break;
        }

        mostrarLineaSeparadora();
        
        #ifdef _WIN32
            setColor(COLOR_CYAN);
        #else
            setColor(COLOR_CYAN);
        #endif
        cout << "Presione Enter para continuar...";
        #ifdef _WIN32
            setColor(COLOR_RESET);
        #else
            setColor(COLOR_RESET);
        #endif
        
        cin.get();
    }

    return 0;
}