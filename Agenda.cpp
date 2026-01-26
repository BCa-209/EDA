#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

void clean() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

class Nodo {
public:
    static int contador_id; // auto incremento

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

    // Constructor del nodo
    Nodo(int identificador, string nom, string apP, string apM, string tel, string mail, string dir, string p_id, string m_id) {
        id = identificador;  // Usamos el ID proporcionado
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

// Inicializar contador estático
int Nodo::contador_id = 1;

// Clase que representa la lista doblemente enlazada
class Lista {
private:
    Nodo* cabezaptr;

public:
    // Constructor de la lista
    Lista() {
        cabezaptr = nullptr;
    }

    // Agrega al INICIO (Push Front)
    void addIni(string nom, string apP, string apM, string tel, string mail, string dir, string p_id, string m_id) {
        Nodo* nuevo = new Nodo(Nodo::contador_id++, nom, apP, apM, tel, mail, dir, p_id, m_id);

        nuevo->siguiente = cabezaptr;
        if(cabezaptr != nullptr) {
            cabezaptr->anterior = nuevo;
        }
        cabezaptr = nuevo;
    }

    // Agrega al FINAL (Push Back)
    void add(string nom, string apP, string apM, string tel, string mail, string dir, string p_id, string m_id) {
        Nodo* nuevo = new Nodo(Nodo::contador_id++, nom, apP, apM, tel, mail, dir, p_id, m_id);

        // Caso 1: Si la lista está vacía
        if (cabezaptr == nullptr) {
            cabezaptr = nuevo;
            return;
        }

        // Caso 2: Recorrer hasta el último nodo
        Nodo* temp = cabezaptr;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }

        temp->siguiente = nuevo;
        nuevo->anterior = temp;
    }

    // Agrega en posición específica
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

    // Eliminar el PRIMER nodo
    void dropIni() {
        if (cabezaptr == nullptr) return;
        Nodo* temp = cabezaptr;
        cabezaptr = cabezaptr->siguiente;
        
        if (cabezaptr != nullptr) {
            cabezaptr->anterior = nullptr;
        }
        
        delete temp;
    }
    
    // Eliminar el ÚLTIMO nodo
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
    
    // Eliminar un nodo en una POSICIÓN específica
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
    
    // Buscar nodo por ID
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
    
    // Buscar nodos por nombre (puede haber varios con el mismo nombre)
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
    
    // Obtener cabeza
    Nodo* getCabeza() {
        return cabezaptr;
    }
    
    // Obtener cantidad de elementos
    int getCantidad() {
        int count = 0;
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            count++;
            actual = actual->siguiente;
        }
        return count;
    }
    
    // Imprimir lista
    void print() {
        Nodo* actual = cabezaptr;
        
        if (actual == nullptr) {
            cout << "La agenda está vacía." << endl;
            return;
        }
        
        cout << "\n=== CONTACTOS EN LA AGENDA ===\n" << endl;
        while (actual != nullptr) {
            cout << "ID: " << actual->id << endl;
            cout << "Nombre: " << actual->nombres << " "
            << actual->apellidoP << " "
            << actual->apellidoM << endl;
            cout << "Telefono: " << actual->telefonos << endl;
            cout << "Correo: " << actual->correo << endl;
            cout << "Direccion: " << actual->direccion << endl;
            cout << "Padre ID: " << actual->padre_id
            << " | Madre ID: " << actual->madre_id << endl;
            cout << "--------------------------" << endl;
            
            actual = actual->siguiente;
        }
    }
    
    // Destructor
    ~Lista() {
        Nodo* actual = cabezaptr;
        while (actual != nullptr) {
            Nodo* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
};

class Agenda {
    private:
    string nombreArchivo;
    Lista* listaContactos;
    
    
    public:
    Agenda() {
        nombreArchivo = "agenda.txt";
        listaContactos = new Lista();
    }
    
    void guardarString(ofstream& archivo, const string& str) {
        size_t size = str.size();
        archivo.write(reinterpret_cast<char*>(&size), sizeof(size_t));
        archivo.write(str.c_str(), size);
    }
    
    string leerString(ifstream& archivo) {
        size_t size;
        archivo.read(reinterpret_cast<char*>(&size), sizeof(size_t));
        
        string str(size, ' ');
        archivo.read(&str[0], size);
        
        return str;
    }
    // Guardar la lista completa en archivo binario
    void guardarEnArchivo() {
        ofstream archivo(nombreArchivo, ios::binary | ios::trunc);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo para guardar." << endl;
            return;
        }
        
        Nodo* actual = listaContactos->getCabeza();
        while (actual != nullptr) {
            // Guardar cada campo del nodo
            archivo.write(reinterpret_cast<char*>(&actual->id), sizeof(int));
            
            guardarString(archivo, actual->nombres);
            guardarString(archivo, actual->apellidoP);
            guardarString(archivo, actual->apellidoM);
            guardarString(archivo, actual->telefonos);
            guardarString(archivo, actual->correo);
            guardarString(archivo, actual->direccion);
            guardarString(archivo, actual->padre_id);
            guardarString(archivo, actual->madre_id);
            
            actual = actual->siguiente;
        }
        
        archivo.close();
        cout << "Agenda guardada correctamente en " << nombreArchivo << endl;
    }
    
    // Cargar la lista desde archivo binario
    void cargarDesdeArchivo() {
        ifstream archivo(nombreArchivo, ios::binary);
        if (!archivo.is_open()) {
            cout << "No se encontró archivo de agenda existente. Se creará uno nuevo." << endl;
            return;
        }
        
        // Limpiar lista actual
        while (listaContactos->getCabeza() != nullptr) {
            listaContactos->dropIni();
        }
        
        Nodo::contador_id = 1; // Reiniciar contador
        
        while (archivo.peek() != EOF) {
            int id;
            archivo.read(reinterpret_cast<char*>(&id), sizeof(int));
            
            string nombres = leerString(archivo);
            string apellidoP = leerString(archivo);
            string apellidoM = leerString(archivo);
            string telefonos = leerString(archivo);
            string correo = leerString(archivo);
            string direccion = leerString(archivo);
            string padre_id = leerString(archivo);
            string madre_id = leerString(archivo);
            
            listaContactos->add(nombres, apellidoP, apellidoM, telefonos, correo, direccion, padre_id, madre_id);
        }
        
        archivo.close();
        cout << "Agenda cargada correctamente desde " << nombreArchivo << endl;
    }
    
    // Métodos para manipular la lista
    void agregarContacto(string nom, string apP, string apM, string tel, string mail, string dir, string p_id = "", string m_id = "") {
        listaContactos->add(nom, apP, apM, tel, mail, dir, p_id, m_id);
        cout << "Contacto agregado correctamente." << endl;
    }
    
    void eliminarContactoPorID(int id) {
        // Buscar el nodo por ID
        Nodo* actual = listaContactos->getCabeza();
        int pos = 0;
        while (actual != nullptr) {
            if (actual->id == id) {
                listaContactos->dropPos(pos);
                cout << "Contacto con ID " << id << " eliminado." << endl;
                return;
            }
            actual = actual->siguiente;
            pos++;
        }
        cout << "No se encontró contacto con ID " << id << endl;
    }
    
    void buscarContacto(string nombre) {
        vector<Nodo*> resultados = listaContactos->buscarPorNombre(nombre);
        
        if (resultados.empty()) {
            cout << "No se encontraron contactos con el nombre: " << nombre << endl;
            return;
        }
        
        cout << "\n=== RESULTADOS DE BÚSQUEDA ===" << endl;
        for (Nodo* nodo : resultados) {
            cout << "ID: " << nodo->id << endl;
            cout << "Nombre: " << nodo->nombres << " "
            << nodo->apellidoP << " "
            << nodo->apellidoM << endl;
            cout << "Telefono: " << nodo->telefonos << endl;
            cout << "Correo: " << nodo->correo << endl;
            cout << "--------------------------" << endl;
        }
    }
    
    void mostrarAgenda() {
        listaContactos->print();
    }
    
    // Método auxiliar para mostrar estadísticas
    void mostrarEstadisticas() {
        int total = listaContactos->getCantidad();
        cout << "\n=== ESTADÍSTICAS DE LA AGENDA ===" << endl;
        cout << "Total de contactos: " << total << endl;
        cout << "Próximo ID disponible: " << Nodo::contador_id << endl;
    }
    
    ~Agenda() {
        delete listaContactos;
    }
};

int main() {
    Agenda miAgenda;
    int opcion;
    
    // Cargar datos existentes al iniciar
    miAgenda.cargarDesdeArchivo();
    
    while (true) {
        cout << "\n=== SISTEMA DE AGENDA ===" << endl;
        cout << "1. Agregar nuevo contacto" << endl;
        cout << "2. Mostrar todos los contactos" << endl;
        cout << "3. Buscar contacto por nombre" << endl;
        cout << "4. Eliminar contacto por ID" << endl;
        cout << "5. Guardar agenda en archivo" << endl;
        cout << "6. Mostrar estadísticas" << endl;
        cout << "7. Salir y guardar automáticamente" << endl;
        cout << "Seleccione una opción: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                string nom, apP, apM, tel, mail, dir, p_id, m_id;
                cout << "\n=== NUEVO CONTACTO ===" << endl;
                cout << "Nombres: "; getline(cin, nom);
                cout << "Apellido Paterno: "; getline(cin, apP);
                cout << "Apellido Materno: "; getline(cin, apM);
                cout << "Telefono: "; getline(cin, tel);
                cout << "Correo: "; getline(cin, mail);
                cout << "Direccion: "; getline(cin, dir);
                cout << "ID Padre (opcional): "; getline(cin, p_id);
                cout << "ID Madre (opcional): "; getline(cin, m_id);
                
                miAgenda.agregarContacto(nom, apP, apM, tel, mail, dir, p_id, m_id);
                break;
            }
            
            case 2:
                miAgenda.mostrarAgenda();
                break;
                
            case 3: {
                string nombre;
                cout << "Ingrese nombre o parte del nombre a buscar: ";
                getline(cin, nombre);
                miAgenda.buscarContacto(nombre);
                break;
            }
            
            case 4: {
                int id;
                cout << "Ingrese ID del contacto a eliminar: ";
                cin >> id;
                cin.ignore();
                miAgenda.eliminarContactoPorID(id);
                break;
            }
            
            case 5:
                miAgenda.guardarEnArchivo();
                break;
            
            case 6:
                miAgenda.mostrarEstadisticas();
                break;
                
            case 7:
                cout << "Guardando automáticamente..." << endl;
                miAgenda.guardarEnArchivo();
                cout << "Saliendo del programa..." << endl;
                return 0;
                
            default:
                cout << "Opción inválida. Intente nuevamente." << endl;
                break;
        }
        
        cout << "\nPresione Enter para continuar...";
        cin.get();
        clean();
    }

    return 0;
}