#include <iostream>
#include <limits>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "colors.h"  // Incluir el archivo de colores

using namespace std;

// Variables globales para combinaciones de colores
string marco_principal() { return string(BG_BLUE) + string(WHITE); }
string titulo_principal() { return string(BG_BLUE) + string(LBLUE); }
string opcion_normal() { return string(BG_GREEN) + string(BLACK); }
string opcion_peligrosa() { return string(BG_RED) + string(WHITE); }
string encabezado_tabla() { return string(BG_LGREEN) + string(BLACK); }
string datos_contacto() { return string(BG_BLUE) + string(WHITE); }
string mensaje_prompt() { return string(BG_GRAY) + string(GREEN); }
string mensaje_error() { return string(BG_RED) + string(WHITE); }
string opcion_seleccionada() { return string(BG_LBLUE) + string(BLACK); }
string opcion_secundaria() { return string(BG_CYAN) + string(BLACK); }

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mostrarMensaje(const string& mensaje, string color = "") {
    if (color.empty()) color = mensaje_prompt();
    
    cout << marco_principal();
    cout << "+--------------------------------------+\n";
    cout << "|                                      |\n";
    cout << "|  " << color << " " << mensaje;
    for (size_t i = mensaje.length(); i < 36; i++) cout << " ";
    cout << marco_principal() << " |\n";
    cout << "|                                      |\n";
    cout << "+--------------------------------------+\n";
    cout << "|  " << mensaje_prompt() << "Presione ENTER para continuar...      ";
    cout << marco_principal() << " |\n";
    cout << "+--------------------------------------+\n";
    cout << RESET;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void mostrarEncabezado(const string& titulo) {
    cout << marco_principal();
    cout << "+--------------------------------------+\n";
    cout << "|";
    cout << titulo_principal();
    cout << " " << titulo;
    for (size_t i = titulo.length(); i < 36; i++) cout << " ";
    cout << marco_principal();
    cout << "|\n";
    cout << "+--------------------------------------+\n";
    cout << RESET;
}

void mostrarMenuPrincipal() {
    limpiarPantalla();
    
    cout << marco_principal();
    cout << "+--------------------------------------+\n";
    cout << "|";
    cout << titulo_principal();
    cout << "           AGENDA PERSONAL            ";
    cout << marco_principal();
    cout << "|\n";
    cout << "+--------------------------------------+\n";
    cout << "|                                      |\n";
    
    cout << "|  ";
    cout << opcion_normal();
    cout << "[1] Agregar contacto ";
    cout << marco_principal();
    cout << "               |\n";

    cout << "|  ";
    cout << opcion_normal();
    cout << "[2] Mostrar todos los contactos ";
    cout << marco_principal();
    cout << "       |\n";

    cout << "|  ";
    cout << opcion_normal();
    cout << "[3] Buscar contacto por nombre ";
    cout << marco_principal();
    cout << "        |\n";

    cout << "|  ";
    cout << opcion_normal();
    cout << "[4] Editar contacto por ID ";
    cout << marco_principal();
    cout << "            |\n";

    cout << "|  ";
    cout << opcion_normal();
    cout << "[5] Eliminar contacto por ID ";
    cout << marco_principal();
    cout << "          |\n";

    cout << "|  ";
    cout << opcion_normal();
    cout << "[6] Guardar agenda en archivo ";
    cout << marco_principal();
    cout << "        |\n";

    cout << "|  ";
    cout << opcion_normal();
    cout << "[7] Mostrar estadísticas ";
    cout << marco_principal();
    cout << "            |\n";

    cout << "|  ";
    cout << opcion_normal();
    cout << "[8] Mostrar relaciones familiares ";
    cout << marco_principal();
    cout << "     |\n";

    cout << "|  ";
    cout << opcion_normal();
    cout << "[9] Exportar a formato .VCF ";
    cout << marco_principal();
    cout << "         |\n";

    cout << "|  ";
    cout << opcion_peligrosa();
    cout << "[0] Salir y guardar ";
    cout << marco_principal();
    cout << "               |\n";

    cout << "|                                      |\n";
    cout << "+--------------------------------------+\n";
    
    cout << "|  ";
    cout << mensaje_prompt();
    cout << "Seleccione una opcion: ";
    cout << marco_principal();
    cout << "             |\n";
    cout << "+--------------------------------------+\n";
    cout << RESET;
}

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
            mostrarMensaje("La agenda está vacía.", mensaje_prompt());
            return;
        }
        
        limpiarPantalla();
        mostrarEncabezado("CONTACTOS EN LA AGENDA");
        
        cout << marco_principal();
        int cont = 0;
        while (actual != nullptr) {
            cout << "|                                      |\n";
            cout << "|  " << encabezado_tabla() << "Contacto #" << ++cont << " - ID: " << actual->id;
            for (size_t i = to_string(cont).length() + to_string(actual->id).length() + 19; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
            cout << "|                                      |\n";
            
            cout << "|  " << datos_contacto() << "Nombre: " << actual->nombres << " " << actual->apellidoP;
            for (size_t i = actual->nombres.length() + actual->apellidoP.length() + 9; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
            
            cout << "|  " << datos_contacto() << "Tel: " << actual->telefonos;
            for (size_t i = actual->telefonos.length() + 5; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
            
            actual = actual->siguiente;
        }
        
        cout << "|                                      |\n";
        cout << "+--------------------------------------+\n";
        cout << "|  " << mensaje_prompt() << "Total contactos: " << cont;
        for (size_t i = to_string(cont).length() + 17; i < 36; i++) cout << " ";
        cout << marco_principal() << " |\n";
        cout << "+--------------------------------------+\n";
        cout << RESET;
        
        cout << mensaje_prompt() << "Presione ENTER para continuar... " << RESET;
        cin.ignore();
        cin.get();
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
            mostrarMensaje("Error al abrir el archivo para guardar.", mensaje_error());
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
        mostrarMensaje("Agenda guardada correctamente.", opcion_normal());
    }

    void cargarDesdeArchivo() {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
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
    }

    void agregarContacto(string nom, string apP, string apM, string tel, string mail, string dir, string p_id = "", string m_id = "") {
        listaContactos->add(nom, apP, apM, tel, mail, dir, p_id, m_id);
        mostrarMensaje("Contacto agregado correctamente.", opcion_normal());
    }
    
    void mostrarListaRapida() {
        Nodo* actual = listaContactos->getCabeza();
        if (actual == nullptr) {
            mostrarMensaje("Agenda vacía - No hay contactos disponibles.", mensaje_prompt());
            return;
        }
        
        limpiarPantalla();
        mostrarEncabezado("CONTACTOS DISPONIBLES");
        
        cout << marco_principal();
        while (actual != nullptr) {
            cout << "|  " << datos_contacto() << "ID: " << actual->id << " -> " 
                 << actual->nombres << " " << actual->apellidoP;
            for (size_t i = to_string(actual->id).length() + actual->nombres.length() + actual->apellidoP.length() + 7; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
            actual = actual->siguiente;
        }
        cout << "+--------------------------------------+\n";
        cout << RESET;
    }

    void editarContactoPorID(int id) {
        Nodo* actual = listaContactos->getCabeza();

        while (actual != nullptr) {
            if (actual->id == id) {
                limpiarPantalla();
                mostrarEncabezado("EDITAR CONTACTO ID " + to_string(id));
                
                cout << marco_principal();
                cout << "|  " << mensaje_prompt() << "Deja vacío para mantener valor actual";
                for (size_t i = 39; i < 36; i++) cout << " ";
                cout << marco_principal() << " |\n";
                cout << "|                                      |\n";
                cout << RESET;

                string input;

                cout << datos_contacto() << "Nombre actual: " << actual->nombres << RESET << endl;
                cout << mensaje_prompt() << "Nuevo nombre: " << RESET;
                getline(cin, input);
                if (!input.empty()) actual->nombres = input;

                cout << datos_contacto() << "Apellido Paterno actual: " << actual->apellidoP << RESET << endl;
                cout << mensaje_prompt() << "Nuevo apellido paterno: " << RESET;
                getline(cin, input);
                if (!input.empty()) actual->apellidoP = input;
                
                cout << datos_contacto() << "Apellido Materno actual: " << actual->apellidoM << RESET << endl;
                cout << mensaje_prompt() << "Nuevo apellido materno: " << RESET;
                getline(cin, input);
                if (!input.empty()) actual->apellidoM = input;

                cout << datos_contacto() << "Teléfonos actuales: " << actual->telefonos << RESET << endl;
                cout << mensaje_prompt() << "Nuevos teléfonos: " << RESET;
                getline(cin, input);
                if (!input.empty()) actual->telefonos = input;

                cout << datos_contacto() << "Correo actual: " << actual->correo << RESET << endl;
                cout << mensaje_prompt() << "Nuevo correo: " << RESET;
                getline(cin, input);
                if (!input.empty()) actual->correo = input;

                cout << datos_contacto() << "Direccion actual: " << actual->direccion << RESET << endl;
                cout << mensaje_prompt() << "Nueva direccion: " << RESET;
                getline(cin, input);
                if (!input.empty()) actual->direccion = input;

                mostrarListaRapida();

                do {
                    cout << datos_contacto() << "Padre ID actual: " << actual->padre_id << RESET << endl;
                    cout << mensaje_prompt() << "Nuevo Padre ID: " << RESET;
                    getline(cin, input);
                    
                    if (input.empty()) break;

                    if (input == to_string(id)) {
                        mostrarMensaje("Error: Un contacto no puede ser su propio padre.", mensaje_error());
                    } else if (!existeID(input)) {
                        mostrarMensaje("Error: El ID " + input + " no existe.", mensaje_error());
                    } else {
                        actual->padre_id = input;
                        break;
                    }
                } while (true);

                do {
                    cout << datos_contacto() << "Madre ID actual: " << actual->madre_id << RESET << endl;
                    cout << mensaje_prompt() << "Nuevo Madre ID: " << RESET;
                    getline(cin, input);

                    if (input.empty()) break;

                    if (input == to_string(id)) {
                        mostrarMensaje("Error: Un contacto no puede ser su propia madre.", mensaje_error());
                    } else if (!existeID(input)) {
                        mostrarMensaje("Error: El ID " + input + " no existe.", mensaje_error());
                    } else {
                        actual->madre_id = input;
                        break;
                    }
                } while (true);

                mostrarMensaje("Contacto actualizado correctamente.", opcion_normal());
                return;
            }
            actual = actual->siguiente;
        }
        mostrarMensaje("No se encontro contacto con ID " + to_string(id), mensaje_error());
    }

    void eliminarContactoPorID(int id) {
        Nodo* actual = listaContactos->getCabeza();
        int pos = 0;
        while (actual != nullptr) {
            if (actual->id == id) {
                listaContactos->dropPos(pos);
                mostrarMensaje("Contacto con ID " + to_string(id) + " eliminado.", opcion_normal());
                return;
            }
            actual = actual->siguiente;
            pos++;
        }
        mostrarMensaje("No se encontro contacto con ID " + to_string(id), mensaje_error());
    }
    
    void buscarContacto(string nombre) {
        vector<Nodo*> resultados = listaContactos->buscarPorNombre(nombre);
        
        if (resultados.empty()) {
            mostrarMensaje("No se encontraron contactos con: " + nombre, mensaje_prompt());
            return;
        }
        
        limpiarPantalla();
        mostrarEncabezado("RESULTADOS DE BÚSQUEDA");
        
        cout << marco_principal();
        for (Nodo* nodo : resultados) {
            cout << "|                                      |\n";
            cout << "|  " << datos_contacto() << "ID: " << nodo->id;
            for (size_t i = to_string(nodo->id).length() + 4; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
            
            cout << "|  " << datos_contacto() << "Nombre: " << nodo->nombres << " " << nodo->apellidoP;
            for (size_t i = nodo->nombres.length() + nodo->apellidoP.length() + 9; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
            
            cout << "|  " << datos_contacto() << "Tel: " << nodo->telefonos;
            for (size_t i = nodo->telefonos.length() + 5; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
        }
        cout << "|                                      |\n";
        cout << "+--------------------------------------+\n";
        cout << "|  " << mensaje_prompt() << "Total encontrados: " << resultados.size();
        for (size_t i = to_string(resultados.size()).length() + 19; i < 36; i++) cout << " ";
        cout << marco_principal() << " |\n";
        cout << "+--------------------------------------+\n";
        cout << RESET;
        
        cout << mensaje_prompt() << "Presione ENTER para continuar... " << RESET;
        cin.ignore();
        cin.get();
    }
    
    void mostrarAgenda() {
        listaContactos->print();
    }
    
    void mostrarEstadisticas() {
        int total = listaContactos->getCantidad();
        
        limpiarPantalla();
        mostrarEncabezado("ESTADÍSTICAS DE LA AGENDA");
        
        cout << marco_principal();
        cout << "|                                      |\n";
        cout << "|  " << datos_contacto() << "Total de contactos: " << total;
        for (size_t i = to_string(total).length() + 20; i < 36; i++) cout << " ";
        cout << marco_principal() << " |\n";
        
        cout << "|  " << datos_contacto() << "Proximo ID disponible: " << Nodo::contador_id;
        for (size_t i = to_string(Nodo::contador_id).length() + 24; i < 36; i++) cout << " ";
        cout << marco_principal() << " |\n";
        cout << "|                                      |\n";
        cout << "+--------------------------------------+\n";
        cout << RESET;
        
        cout << mensaje_prompt() << "Presione ENTER para continuar... " << RESET;
        cin.ignore();
        cin.get();
    }
    
    void mostrarRelacionesFamiliares(int id) {
        Nodo* persona = listaContactos->buscarPorID(id);
        if (persona == nullptr) {
            mostrarMensaje("Contacto con ID " + to_string(id) + " no encontrado.", mensaje_error());
            return;
        }

        limpiarPantalla();
        mostrarEncabezado("ÁRBOL FAMILIAR DE: " + persona->nombres + " " + persona->apellidoP);
        
        cout << marco_principal();
        cout << "|                                      |\n";
        cout << "|  " << encabezado_tabla() << "PADRES:";
        for (size_t i = 7; i < 36; i++) cout << " ";
        cout << marco_principal() << " |\n";
        cout << "|                                      |\n";
        
        if (!persona->padre_id.empty()) {
            Nodo* p = listaContactos->buscarPorID(stoi(persona->padre_id));
            cout << "|  " << datos_contacto() << "Padre: " << (p ? p->nombres + " " + p->apellidoP : "ID " + persona->padre_id);
            for (size_t i = (p ? p->nombres.length() + p->apellidoP.length() + 7 : persona->padre_id.length() + 6); i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
        } else {
            cout << "|  " << datos_contacto() << "Padre: No registrado";
            for (size_t i = 23; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
        }

        if (!persona->madre_id.empty()) {
            Nodo* m = listaContactos->buscarPorID(stoi(persona->madre_id));
            cout << "|  " << datos_contacto() << "Madre: " << (m ? m->nombres + " " + m->apellidoP : "ID " + persona->madre_id);
            for (size_t i = (m ? m->nombres.length() + m->apellidoP.length() + 7 : persona->madre_id.length() + 6); i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
        } else {
            cout << "|  " << datos_contacto() << "Madre: No registrada";
            for (size_t i = 24; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
        }

        cout << "|                                      |\n";
        cout << "|  " << encabezado_tabla() << "HIJOS:";
        for (size_t i = 6; i < 36; i++) cout << " ";
        cout << marco_principal() << " |\n";
        cout << "|                                      |\n";

        bool tieneHijos = false;
        Nodo* actual = listaContactos->getCabeza();
        string idBuscado = to_string(id);

        while (actual != nullptr) {
            if (actual->padre_id == idBuscado || actual->madre_id == idBuscado) {
                cout << "|  " << datos_contacto() << "* " << actual->nombres << " " << actual->apellidoP;
                for (size_t i = actual->nombres.length() + actual->apellidoP.length() + 3; i < 36; i++) cout << " ";
                cout << marco_principal() << " |\n";
                tieneHijos = true;
            }
            actual = actual->siguiente;
        }

        if (!tieneHijos) {
            cout << "|  " << datos_contacto() << "(No se encontraron hijos)";
            for (size_t i = 30; i < 36; i++) cout << " ";
            cout << marco_principal() << " |\n";
        }
        
        cout << "|                                      |\n";
        cout << "+--------------------------------------+\n";
        cout << RESET;
        
        cout << mensaje_prompt() << "Presione ENTER para continuar... " << RESET;
        cin.ignore();
        cin.get();
    }

    void exportarVCF() {
        string nombreVCF = "contactos_exportados.vcf";
        ofstream archivo(nombreVCF);
        
        if (!archivo.is_open()) {
            mostrarMensaje("Error al crear archivo VCF.", mensaje_error());
            return;
        }

        Nodo* actual = listaContactos->getCabeza();
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
        }

        archivo.close();
        mostrarMensaje("Archivo VCF generado exitosamente.", opcion_normal());
    }

    ~Agenda() {
        delete listaContactos;
    }
};

int main() {
    Agenda miAgenda;
    int opcion;

    miAgenda.cargarDesdeArchivo();

    do {
        mostrarMenuPrincipal();
        
        cout << mensaje_prompt() << "   -> " << RESET;
        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            limpiarPantalla();
            continue;
        }
        cin.ignore();

        switch (opcion) {
            case 1: {
                limpiarPantalla();
                mostrarEncabezado("NUEVO CONTACTO");
                
                string nom, apP, apM, tel, mail, dir, p_id, m_id;

                cout << mensaje_prompt() << "Nombres: " << RESET;
                getline(cin, nom);

                cout << mensaje_prompt() << "Apellido Paterno: " << RESET;
                getline(cin, apP);

                cout << mensaje_prompt() << "Apellido Materno: " << RESET;
                getline(cin, apM);

                cout << mensaje_prompt() << "Teléfonos (separados por ;): " << RESET;
                getline(cin, tel);

                cout << mensaje_prompt() << "Correo: " << RESET;
                getline(cin, mail);

                cout << mensaje_prompt() << "Direccion: " << RESET;
                getline(cin, dir);

                miAgenda.mostrarListaRapida();

                do {
                    cout << mensaje_prompt() << "ID del Padre (Opcional): " << RESET;
                    getline(cin, p_id);
                    if (p_id.empty() || miAgenda.existeID(p_id)) {
                        break;
                    } else {
                        mostrarMensaje("Error: El ID " + p_id + " no existe.", mensaje_error());
                    }
                } while (true);
                
                do {
                    cout << mensaje_prompt() << "ID de la Madre (Opcional): " << RESET;
                    getline(cin, m_id);
                    if (m_id.empty() || miAgenda.existeID(m_id)) {
                        break;
                    } else {
                        mostrarMensaje("Error: El ID " + m_id + " no existe.", mensaje_error());
                    }
                } while (true);

                miAgenda.agregarContacto(nom, apP, apM, tel, mail, dir, p_id, m_id);
                break;
            }

            case 2:
                miAgenda.mostrarAgenda();
                break;

            case 3: {
                string nombre;
                limpiarPantalla();
                mostrarEncabezado("BUSCAR CONTACTO");
                cout << mensaje_prompt() << "Ingrese nombre o parte del nombre: " << RESET;
                getline(cin, nombre);
                miAgenda.buscarContacto(nombre);
                break;
            }

            case 4: {
                int id;
                miAgenda.mostrarListaRapida();
                cout << mensaje_prompt() << "ID del contacto a editar: " << RESET;
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    mostrarMensaje("ID inválido.", mensaje_error());
                    break;
                }
                cin.ignore();
                miAgenda.editarContactoPorID(id);
                break;
            }

            case 5: {
                int id;
                miAgenda.mostrarListaRapida();
                cout << mensaje_prompt() << "ID del contacto a eliminar: " << RESET;
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    mostrarMensaje("ID inválido.", mensaje_error());
                    break;
                }
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

            case 8: {
                int id;
                miAgenda.mostrarListaRapida();
                cout << mensaje_prompt() << "ID para ver familia: " << RESET;
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    mostrarMensaje("ID inválido.", mensaje_error());
                    break;
                }
                cin.ignore();
                miAgenda.mostrarRelacionesFamiliares(id);
                break;
            }

            case 9:
                miAgenda.exportarVCF();
                break;
                
            case 0:
                limpiarPantalla();
                miAgenda.guardarEnArchivo();
                mostrarEncabezado("SALIENDO DE LA AGENDA");
                cout << marco_principal();
                cout << "|                                      |\n";
                cout << "|  " << mensaje_prompt() << "¡Gracias por usar la agenda!         ";
                cout << marco_principal() << " |\n";
                cout << "|                                      |\n";
                cout << "+--------------------------------------+\n";
                cout << RESET;
                break;

            default:
                mostrarMensaje("Opcion inválida. Intente nuevamente.", mensaje_error());
                break;
        }

    } while (opcion != 0);

    return 0;
}