#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>
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
//class persona
  //  string telefonos;
  //  string correo;
  //  string direccion;
  //  string padre_id;
  //  
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

// Inicializar contador estatico
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

        // Caso 1: Si la lista esta vacia
        if (cabezaptr == nullptr) {
            cabezaptr = nuevo;
            return;
        }

        // Caso 2: Recorrer hasta el Ultimo nodo
        Nodo* temp = cabezaptr;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }

        temp->siguiente = nuevo;
        nuevo->anterior = temp;
    }

    // Agrega en posicion especifica
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
    
    // Eliminar el ULTIMO nodo
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
    
    // Eliminar un nodo en una POSICIoN especifica
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
            cout << "La agenda esta vacia." << endl;
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
    
    bool existeID(string idStr) {
        if (idStr.empty()) return true; // Es valido dejarlo vacío
        try {
            int id = stoi(idStr);
            return listaContactos->buscarPorID(id) != nullptr;
        } catch (...) {
            return false; // Por si ingresan letras en lugar de números
        }
    }
    
    // Guardar la lista completa en archivo binario
    void guardarEnArchivo() {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo para guardar." << endl;
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

            // 🔹 MULTIPLES TELEFONOS
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
        cout << "Agenda guardada correctamente en formato vCard TXT" << endl;
    }

    // Cargar la lista desde archivo binario
    void cargarDesdeArchivo() {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cout << "No se encontro archivo de agenda existente." << endl;
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
                // N:Apellido;Nombre;;;
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
                    apellidoM,               // apellido materno (si no lo usas)
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
        cout << "Agenda cargada correctamente desde vCard TXT" << endl;
    }

    // Métodos para manipular la lista
    void agregarContacto(string nom, string apP, string apM, string tel, string mail, string dir, string p_id = "", string m_id = "") {
        listaContactos->add(nom, apP, apM, tel, mail, dir, p_id, m_id);
        cout << "Contacto agregado correctamente." << endl;
    }
    
    void mostrarListaRapida() {
        Nodo* actual = listaContactos->getCabeza();
        if (actual == nullptr) {
            cout << "[ Agenda vacia - No hay padres para asignar ]" << endl;
            return;
        }
        cout << "\n--- CONTACTOS DISPONIBLES (ID y Nombre) ---" << endl;
        while (actual != nullptr) {
            cout << "ID: " << actual->id << " -> " << actual->nombres << " " << actual->apellidoP << " " << actual->apellidoM << endl;
            actual = actual->siguiente;
        }
        cout << "--------------------------------------------" << endl;
    }

    void editarContactoPorID(int id) {
        Nodo* actual = listaContactos->getCabeza();

        while (actual != nullptr) {
            if (actual->id == id) {
                cout << "\n=== EDITAR CONTACTO ID " << id << " ===\n";
                cout << "(Deja vacio y presiona ENTER para mantener el valor actual)\n\n";

                string input;

                // ... (Nombre, Apellidos, Teléfono, Correo, Dirección se mantienen igual) ...
                cout << "Nombre actual: " << actual->nombres << "\nNuevo nombre: ";
                getline(cin, input);
                if (!input.empty()) actual->nombres = input;

                cout << "Apellido Paterno actual: " << actual->apellidoP << "\nNuevo apellido paterno: ";
                getline(cin, input);
                if (!input.empty()) actual->apellidoP = input;
                
                cout << "Apellido Materno actual: " << actual->apellidoM << "\nNuevo apellido materno: ";
                getline(cin, input);
                if (!input.empty()) actual->apellidoM = input;

                cout << "Telefonos actuales: " << actual->telefonos << "\nNuevos telefonos: ";
                getline(cin, input);
                if (!input.empty()) actual->telefonos = input;

                cout << "Correo actual: " << actual->correo << "\nNuevo correo: ";
                getline(cin, input);
                if (!input.empty()) actual->correo = input;

                cout << "Direccion actual: " << actual->direccion << "\nNueva direccion: ";
                getline(cin, input);
                if (!input.empty()) actual->direccion = input;

                // MOSTRAR REFERENCIA DE CONTACTOS
                mostrarListaRapida();

                // VALIDACIÓN PARA PADRE
                do {
                    cout << "Padre ID actual: " << actual->padre_id << "\nNuevo Padre ID: ";
                    getline(cin, input);
                    
                    if (input.empty()) break; // No se desea cambiar

                    if (input == to_string(id)) {
                        cout << "Error: Un contacto no puede ser su propio padre.\n";
                    } else if (!existeID(input)) {
                        cout << "Error: El ID " << input << " no existe.\n";
                    } else {
                        actual->padre_id = input;
                        break;
                    }
                } while (true);

                // VALIDACIÓN PARA MADRE
                do {
                    cout << "Madre ID actual: " << actual->madre_id << "\nNuevo Madre ID: ";
                    getline(cin, input);

                    if (input.empty()) break; // No se desea cambiar

                    if (input == to_string(id)) {
                        cout << "Error: Un contacto no puede ser su propia madre.\n";
                    } else if (!existeID(input)) {
                        cout << "Error: El ID " << input << " no existe.\n";
                    } else {
                        actual->madre_id = input;
                        break;
                    }
                } while (true);

                cout << "\nContacto actualizado correctamente \n";
                return;
            }
            actual = actual->siguiente;
        }
        cout << "No se encontro contacto con ID " << id << endl;
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
        cout << "No se encontro contacto con ID " << id << endl;
    }
    
    void buscarContacto(string nombre) {
        vector<Nodo*> resultados = listaContactos->buscarPorNombre(nombre);
        
        if (resultados.empty()) {
            cout << "No se encontraron contactos con el nombre: " << nombre << endl;
            return;
        }
        
        cout << "\n=== RESULTADOS DE BUSQUEDA ===" << endl;
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
    
    // Método auxiliar para mostrar estadisticas
    void mostrarEstadisticas() {
        int total = listaContactos->getCantidad();
        cout << "\n=== ESTADiSTICAS DE LA AGENDA ===" << endl;
        cout << "Total de contactos: " << total << endl;
        cout << "Proximo ID disponible: " << Nodo::contador_id << endl;
    }
    
    void mostrarRelacionesFamiliares(int id) {
        // 1. Buscar a la persona principal
        Nodo* persona = listaContactos->buscarPorID(id);
        if (persona == nullptr) {
            cout << "Contacto con ID " << id << " no encontrado." << endl;
            return;
        }

        cout << "\n==========================================" << endl;
        cout << "   ARBOL FAMILIAR DE: " << persona->nombres << " " << persona->apellidoP << endl;
        cout << "==========================================" << endl;

        // 2. BUSCAR PADRES (Hacia arriba)
        cout << "PADRES:" << endl;
        if (!persona->padre_id.empty()) {
            Nodo* p = listaContactos->buscarPorID(stoi(persona->padre_id));
            cout << "  - Padre: " << (p ? p->nombres + " " + p->apellidoP + " (ID: " + to_string(p->id) + ")" : "ID " + persona->padre_id + " [No en agenda]") << endl;
        } else {
            cout << "  - Padre: No registrado" << endl;
        }

        if (!persona->madre_id.empty()) {
            Nodo* m = listaContactos->buscarPorID(stoi(persona->madre_id));
            cout << "  - Madre: " << (m ? m->nombres + " " + m->apellidoP + " (ID: " + to_string(m->id) + ")" : "ID " + persona->madre_id + " [No en agenda]") << endl;
        } else {
            cout << "  - Madre: No registrada" << endl;
        }

        // 3. BUSCAR HIJOS (Hacia abajo - Escaneo de la lista)
        cout << "\nHIJOS:" << endl;
        bool tieneHijos = false;
        Nodo* actual = listaContactos->getCabeza();
        string idBuscado = to_string(id);

        while (actual != nullptr) {
            if (actual->padre_id == idBuscado || actual->madre_id == idBuscado) {
                cout << "  * " << actual->nombres << " " << actual->apellidoP << " (ID: " << actual->id << ")" << endl;
                tieneHijos = true;
            }
            actual = actual->siguiente;
        }

        if (!tieneHijos) {
            cout << "  (No se encontraron hijos registrados)" << endl;
        }
        cout << "==========================================" << endl;
    }

    void exportarVCF() {
        string nombreVCF = "contactos_exportados.vcf";
        ofstream archivo(nombreVCF);
        
        if (!archivo.is_open()) {
            cout << "Error al crear el archivo VCF." << endl;
            return;
        }

        Nodo* actual = listaContactos->getCabeza();
        while (actual != nullptr) {
            archivo << "BEGIN:VCARD\n";
            archivo << "VERSION:2.1\n";
            // Formato N: ApellidoP;Nombre;ApellidoM;;;
            archivo << "N:" << actual->apellidoP << ";" 
                    << actual->nombres << ";" 
                    << actual->apellidoM << ";;;\n";
            archivo << "FN:" << actual->nombres << " " 
                    << actual->apellidoP << "\n";

            // Exportar teléfonos (separados por ; en tu sistema)
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
        }

        archivo.close();
        cout << "\nArchivo '" << nombreVCF << "' generado con exito." << endl;
        cout << "Puedes importar este archivo directamente en tu celular." << endl;
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
        cout << "4. Editar contacto por ID" << endl;
        cout << "5. Eliminar contacto por ID" << endl;
        cout << "6. Guardar agenda en archivo" << endl;
        cout << "7. Mostrar estadisticas" << endl;
        cout << "8. Mostrar relaciones familiares (padres-hijos)" << endl;
        cout << "9. Exportar a formato .VCF (Celulares)" << endl;
        cout << "0. Salir y guardar automaticamente" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {

            case 1: {
                string nom, apP, apM, tel, mail, dir, p_id, m_id;

                cout << "\n=== NUEVO CONTACTO ===" << endl;
                cout << "Nombres: ";
                getline(cin, nom);

                cout << "Apellido Paterno: ";
                getline(cin, apP);

                cout << "Apellido Materno: ";
                getline(cin, apM);

                cout << "Telefonos (separados por ;): ";
                getline(cin, tel);

                cout << "Correo: ";
                getline(cin, mail);

                cout << "Direccion: ";
                getline(cin, dir);

                miAgenda.mostrarListaRapida();

                do {
                    cout << "ID del Padre (Opcional): ";
                    getline(cin, p_id);
                    if (!miAgenda.existeID(p_id)) {
                        cout << "Error: El ID " << p_id << " no existe. Intenta de nuevo." << endl;
                    } else {
                        break;
                    }
                } while (true);
                
                do {
                    cout << "ID de la Madre (Opcional): ";
                    getline(cin, m_id);
                    if (!miAgenda.existeID(m_id)) {
                        cout << "Error: El ID " << m_id << " no existe. Intenta de nuevo." << endl;
                    } else {
                        break;
                    }
                } while (true);

                miAgenda.agregarContacto(
                    nom, apP, apM, tel, mail, dir, p_id, m_id
                );
                break;
            }

            case 2:
                miAgenda.mostrarAgenda();
                break;

            case 3: {
                string nombre;
                cout << "\nIngrese nombre o parte del nombre a buscar: ";
                getline(cin, nombre);
                miAgenda.buscarContacto(nombre);
                break;
            }

            case 4: {
                int id;
                miAgenda.mostrarListaRapida();
                cout << "\nIngrese ID del contacto a editar: ";
                cin >> id;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                miAgenda.editarContactoPorID(id);
                break;
            }

            case 5: {
                int id;
                miAgenda.mostrarListaRapida();
                cout << "\nIngrese ID del contacto a eliminar: ";
                cin >> id;
                cin.ignore();
                miAgenda.eliminarContactoPorID(id);
                break;
            }

            case 6:
                miAgenda.guardarEnArchivo();
                break;

            case 7:
                miAgenda.mostrarEstadisticas();
                break;

            case 8:
                int id;
                miAgenda.mostrarListaRapida();
                cout << "\nIngrese el ID del contacto para ver su familia: ";
                cin >> id;
                cin.ignore();
                miAgenda.mostrarRelacionesFamiliares(id);
                break;

            case 9:
                miAgenda.exportarVCF();
                break;
                
            case 0:
                cout << "\nGuardando automaticamente..." << endl;
                miAgenda.guardarEnArchivo();
                cout << "Saliendo del programa..." << endl;
                return 0;

            default:
                cout << "\nOpcion invalida. Intente nuevamente." << endl;
                break;
        }

        cout << "\nPresione Enter para continuar...";
        cin.get();
        clean();
    }

    return 0;
}

