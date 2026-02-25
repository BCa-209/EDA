#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <vector>
#include <limits>
#include <map>
#include <string>
using namespace std;

// Clase Grupo
class Grupo {
private:
    string nombre;
    vector<char> puntos;
    string color;
    
public:
    Grupo() {}
    
    Grupo(string _nombre, string _color) : nombre(_nombre), color(_color) {}
    
    string getNombre() const { return nombre; }
    string getColor() const { return color; }
    vector<char> getPuntos() const { return puntos; }
    
    void setNombre(string _nombre) { nombre = _nombre; }
    void setColor(string _color) { color = _color; }
    
    void agregarPunto(char punto) {
        if(!tienePunto(punto)) {
            puntos.push_back(punto);
        }
    }
    
    bool tienePunto(char punto) const {
        return find(puntos.begin(), puntos.end(), punto) != puntos.end();
    }
    
    int cantidadPuntos() const {
        return puntos.size();
    }
    
    void eliminarPunto(char punto) {
        auto it = find(puntos.begin(), puntos.end(), punto);
        if(it != puntos.end()) {
            puntos.erase(it);
        }
    }
};

class Plano {
private:
    int maxX;
    int maxY;
    
    struct Punto {
        char nombre;
        int x;
        int y;
        string grupo;  // Grupo al que pertenece
    };

    vector<Punto> puntos;
    vector<Grupo> grupos;
    vector<string> colores;
    string colorReset = "\033[0m";

public:
    Plano() {
        colores = {
            "\033[31m", // Rojo
            "\033[32m", // Verde
            "\033[33m", // Amarillo
            "\033[34m", // Azul
            "\033[35m", // Magenta
            "\033[36m", // Cian
            "\033[91m", // Rojo claro
            "\033[92m", // Verde claro
            "\033[93m", // Amarillo claro
            "\033[94m"  // Azul claro
        };
    }

    void limites(int x, int y) {
        maxX = x;
        maxY = y;
    }

    // Metodo para crear grupos base
    void crearGruposBase() {
        grupos.clear();
        grupos.push_back(Grupo("Bajo Peso", colores[0]));
        grupos.push_back(Grupo("Normal", colores[1]));
        grupos.push_back(Grupo("Sobrepeso", colores[2]));
        grupos.push_back(Grupo("Obesidad", colores[3]));
        cout << "\n=== Grupos base creados ===\n";
    }

    // Metodo para mostrar grupos disponibles
    void mostrarGruposDisponibles() {
        cout << "Grupos disponibles:\n";
        for(size_t i = 0; i < grupos.size(); i++) {
            cout << "  " << i+1 << ". " << grupos[i].getNombre() << endl;
        }
    }

    // TODO: input(x,y,Nombre;Grupo) - Implementacion del aprendizaje supervisado
    void agregarPuntoConGrupo(char nombre, int x, int y, string nombreGrupo) {
        // Validar rango
        if(x < 0 || x > maxX || y < 0 || y > maxY) {
            cout << "Error: Punto fuera del rango permitido\n";
            return;
        }

        // Validar nombre repetido
        for(const auto& p : puntos) {
            if(p.nombre == nombre) {
                cout << "Ya existe un punto con nombre " << nombre << endl;
                return;
            }
        }

        // Verificar que el grupo existe
        bool grupoValido = false;
        for(const auto& g : grupos) {
            if(g.getNombre() == nombreGrupo) {
                grupoValido = true;
                break;
            }
        }

        if(!grupoValido) {
            cout << "Error: El grupo '" << nombreGrupo << "' no existe\n";
            mostrarGruposDisponibles();
            return;
        }

        // Agregar el punto con su grupo
        puntos.push_back({nombre, x, y, nombreGrupo});
        
        // Asignar el punto al grupo correspondiente
        for(auto& g : grupos) {
            if(g.getNombre() == nombreGrupo) {
                g.agregarPunto(nombre);
                break;
            }
        }

        cout << "\n=== Punto " << nombre << " agregado al grupo '" 
             << nombreGrupo << "' ===\n";
    }

    // Metodo para clasificar un nuevo punto usando KNN (k=1)
    void clasificarPuntoKNN(char nombre, int x, int y) {
        // Validar rango
        if(x < 0 || x > maxX || y < 0 || y > maxY) {
            cout << "Error: Punto fuera del rango permitido\n";
            return;
        }

        // Validar nombre repetido
        for(const auto& p : puntos) {
            if(p.nombre == nombre) {
                cout << "Ya existe un punto con nombre " << nombre << endl;
                return;
            }
        }

        if(puntos.empty()) {
            cout << "No hay puntos de referencia para clasificar\n";
            return;
        }

        if(grupos.empty()) {
            cout << "No hay grupos definidos\n";
            return;
        }

        // Calcular distancias a todos los puntos existentes
        vector<pair<double, Punto>> distancias;
        
        for(const auto& p : puntos) {
            double dist = sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
            distancias.push_back({dist, p});
        }

        // Ordenar por distancia usando lambda function
        sort(distancias.begin(), distancias.end(), 
            [](const pair<double, Punto>& a, const pair<double, Punto>& b) {
                return a.first < b.first;
            });

        // El punto mas cercano (K=1)
        Punto puntoCercano = distancias[0].second;
        string grupoAsignado = puntoCercano.grupo;

        // Agregar el nuevo punto con el grupo del mas cercano
        puntos.push_back({nombre, x, y, grupoAsignado});
        
        // Asignar al grupo correspondiente
        for(auto& g : grupos) {
            if(g.getNombre() == grupoAsignado) {
                g.agregarPunto(nombre);
                break;
            }
        }

        cout << "\n=== PUNTO CLASIFICADO (KNN) ===\n";
        cout << "Punto " << nombre << " (" << x << "," << y << ")\n";
        cout << "Punto mas cercano: " << puntoCercano.nombre 
             << " (" << puntoCercano.x << "," << puntoCercano.y << ")\n";
        cout << "Distancia: " << distancias[0].first << endl;
        cout << "Grupo asignado: " << grupoAsignado << endl;
        
        // Mostrar los 3 puntos mas cercanos (vecinos)
        cout << "\nVecinos mas cercanos:\n";
        for(int i = 0; i < min(3, (int)distancias.size()); i++) {
            cout << i+1 << ". " << distancias[i].second.nombre 
                 << " (Grupo: " << distancias[i].second.grupo 
                 << ", Distancia: " << distancias[i].first << ")\n";
        }
    }

    // Metodo para clasificar con K mayor (opcional)
    void clasificarPuntoKNN_MayorK(char nombre, int x, int y, int k = 3) {
        // Validar rango
        if(x < 0 || x > maxX || y < 0 || y > maxY) {
            cout << "Error: Punto fuera del rango permitido\n";
            return;
        }

        // Validar nombre repetido
        for(const auto& p : puntos) {
            if(p.nombre == nombre) {
                cout << "Ya existe un punto con nombre " << nombre << endl;
                return;
            }
        }

        if(puntos.empty()) {
            cout << "No hay puntos de referencia para clasificar\n";
            return;
        }

        if(grupos.empty()) {
            cout << "No hay grupos definidos\n";
            return;
        }

        // Asegurar que k no sea mayor que el numero de puntos
        if((int)puntos.size() < k) {
            cout << "No hay suficientes puntos para K=" << k 
                 << " (solo hay " << puntos.size() << " puntos)\n";
            k = puntos.size();
            cout << "Usando K=" << k << " en su lugar\n";
        }

        // Calcular distancias
        vector<pair<double, Punto>> distancias;
        for(const auto& p : puntos) {
            double dist = sqrt(pow(p.x - x, 2) + pow(p.y - y, 2));
            distancias.push_back({dist, p});
        }

        // Ordenar por distancia usando lambda function
        sort(distancias.begin(), distancias.end(), 
            [](const pair<double, Punto>& a, const pair<double, Punto>& b) {
                return a.first < b.first;
            });

        // Votacion entre los K vecinos
        map<string, int> votos;
        for(int i = 0; i < k; i++) {
            votos[distancias[i].second.grupo]++;
        }

        // Encontrar grupo con mas votos
        string grupoAsignado;
        int maxVotos = -1;
        for(const auto& voto : votos) {
            if(voto.second > maxVotos) {
                maxVotos = voto.second;
                grupoAsignado = voto.first;
            }
        }

        // Agregar punto
        puntos.push_back({nombre, x, y, grupoAsignado});
        
        for(auto& g : grupos) {
            if(g.getNombre() == grupoAsignado) {
                g.agregarPunto(nombre);
                break;
            }
        }

        cout << "\n=== PUNTO CLASIFICADO (K=" << k << " KNN) ===\n";
        cout << "Punto " << nombre << " (" << x << "," << y << ")\n";
        cout << "Resultado votacion:\n";
        for(const auto& voto : votos) {
            cout << "  " << voto.first << ": " << voto.second << " votos\n";
        }
        cout << "Grupo asignado: " << grupoAsignado << endl;
        
        // Mostrar los K vecinos
        cout << "\nLos " << k << " vecinos mas cercanos:\n";
        for(int i = 0; i < k; i++) {
            cout << i+1 << ". " << distancias[i].second.nombre 
                 << " (" << distancias[i].second.x << "," << distancias[i].second.y
                 << ") Grupo: " << distancias[i].second.grupo
                 << ", Distancia: " << distancias[i].first << endl;
        }
    }

    void drawConColores() {
        cout << "\n" << string(60, '=') << endl;
        cout << "           PLANO CARTESIANO CON GRUPOS (KNN)" << endl;
        cout << string(60, '=') << endl;
        
        for(int y = maxY; y >= 0; y--) {
            for(int x = 0; x <= maxX; x++) {
                if(y == 0 && x == 0) {
                    cout << "   +";
                }
                else if(y == 0) {
                    cout << "--";
                }
                else if(x == 0) {
                    cout << setw(2) << y << " |";
                }
                else {
                    char simbolo = '.';
                    string color = colorReset;
                    
                    for(const auto& p : puntos) {
                        if(p.x == x && p.y == y) {
                            simbolo = p.nombre;
                            for(const auto& grupo : grupos) {
                                if(grupo.tienePunto(p.nombre)) {
                                    color = grupo.getColor();
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    
                    if(simbolo != '.') {
                        cout << " " << color << simbolo << colorReset;
                    } else {
                        cout << " " << simbolo;
                    }
                }
            }
            cout << endl;
        }

        // Numeracion eje X
        cout << "    ";
        for(int x = 1; x <= maxX; x++) {
            if(x % 2 == 0) {
                cout << " " << x/10;
            } else {
                cout << "  ";
            }
        }
        cout << endl;
        cout << "    ";
        for(int x = 1; x <= maxX; x++) {
            if(x % 2 == 0) {
                cout << " " << x%10;
            } else {
                cout << "  ";
            }
        }
        cout << endl;
        
        // Mostrar leyenda
        cout << "\nLeyenda de grupos:\n";
        for(const auto& grupo : grupos) {
            cout << grupo.getColor() << "■ " << grupo.getNombre() 
                 << colorReset << " (" << grupo.cantidadPuntos() << " puntos)\n";
        }
    }

    void mostrarPuntosConGrupos() {
        if(puntos.empty()) {
            cout << "No hay puntos registrados\n";
            return;
        }
        
        cout << "\nPuntos con sus grupos:\n";
        cout << string(40, '-') << endl;
        for(const auto& p : puntos) {
            string color = colorReset;
            for(const auto& grupo : grupos) {
                if(grupo.tienePunto(p.nombre)) {
                    color = grupo.getColor();
                    break;
                }
            }
            cout << color << p.nombre << colorReset 
                 << " (" << p.x << "," << p.y << ") -> Grupo: " 
                 << p.grupo << endl;
        }
    }

    bool vacio() const {
        return puntos.empty();
    }
    
    void mostrarGrupos() const {
        if(grupos.empty()) {
            cout << "No hay grupos definidos\n";
            return;
        }
        
        cout << "\n" << string(50, '=') << endl;
        cout << "           GRUPOS EXISTENTES" << endl;
        cout << string(50, '=') << endl;
        
        for(const auto& grupo : grupos) {
            cout << grupo.getColor() << "+ " << grupo.getNombre() 
                 << colorReset << " (" << grupo.cantidadPuntos() << " puntos)\n";
            cout << "   Puntos: ";
            if(grupo.cantidadPuntos() == 0) {
                cout << "(vacio)";
            } else {
                for(char p : grupo.getPuntos()) {
                    for(const auto& punto : puntos) {
                        if(punto.nombre == p) {
                            cout << p << " ";
                            break;
                        }
                    }
                }
            }
            cout << "\n" << string(30, '-') << endl;
        }
    }
};

void clean() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    Plano plano;
    int opcion;
    plano.limites(20, 10);
    
    // Crear grupos base primero
    plano.crearGruposBase();

    do {
        clean();
        cout << "==== MENU PLANO CARTESIANO (KNN) ====\n";
        cout << "1. Definir limites del plano\n";
        cout << "2. Agregar punto con grupo (Supervisado - TODO)\n";
        cout << "3. Clasificar nuevo punto (KNN k=1)\n";
        cout << "4. Clasificar nuevo punto (KNN k=3)\n";
        cout << "5. Mostrar plano con colores\n";
        cout << "6. Mostrar todos los puntos y grupos\n";
        cout << "7. Mostrar grupos\n";
        cout << "8. Salir\n";
        cout << "Seleccione opcion: ";
        cin >> opcion;

        switch(opcion) {
            case 1: {
                int x, y;
                cout << "Ingrese limite X: ";
                cin >> x;
                cout << "Ingrese limite Y: ";
                cin >> y;
                plano.limites(x, y);
                break;
            }
            
            case 2: {
                // Implementacion del TODO: input(x,y,Nombre;Grupo)
                char nom;
                int px, py;
                string grupo;
                int opcionGrupo;
                
                cout << "Ingrese nombre del punto: ";
                cin >> nom;
                cout << "Ingrese coordenada X: ";
                cin >> px;
                cout << "Ingrese coordenada Y: ";
                cin >> py;
                
                cout << "Seleccione grupo:\n";
                cout << "1. Bajo Peso\n";
                cout << "2. Normal\n";
                cout << "3. Sobrepeso\n";
                cout << "4. Obesidad\n";
                cout << "Opcion: ";
                cin >> opcionGrupo;
                
                switch(opcionGrupo) {
                    case 1: grupo = "Bajo Peso"; break;
                    case 2: grupo = "Normal"; break;
                    case 3: grupo = "Sobrepeso"; break;
                    case 4: grupo = "Obesidad"; break;
                    default: 
                        cout << "Opcion invalida\n";
                        cout << "Presione enter...";
                        cin.ignore();
                        cin.get();
                        continue;
                }
                
                plano.agregarPuntoConGrupo(nom, px, py, grupo);
                plano.drawConColores();
                cout << "\nPresione enter...";
                cin.ignore();
                cin.get();
                break;
            }
            
            case 3: {
                char nom;
                int px, py;
                
                cout << "Ingrese nombre del nuevo punto: ";
                cin >> nom;
                cout << "Ingrese coordenada X: ";
                cin >> px;
                cout << "Ingrese coordenada Y: ";
                cin >> py;
                
                plano.clasificarPuntoKNN(nom, px, py);
                plano.drawConColores();
                cout << "\nPresione enter...";
                cin.ignore();
                cin.get();
                break;
            }
            
            case 4: {
                char nom;
                int px, py;
                int k = 3;
                
                cout << "Ingrese nombre del nuevo punto: ";
                cin >> nom;
                cout << "Ingrese coordenada X: ";
                cin >> px;
                cout << "Ingrese coordenada Y: ";
                cin >> py;
                
                cout << "Ingrese valor de K (default 3): ";
                cin >> k;
                
                plano.clasificarPuntoKNN_MayorK(nom, px, py, k);
                plano.drawConColores();
                cout << "\nPresione enter...";
                cin.ignore();
                cin.get();
                break;
            }
            
            case 5:
                plano.drawConColores();
                cout << "\nPresione enter...";
                cin.ignore();
                cin.get();
                break;
                
            case 6:
                plano.mostrarPuntosConGrupos();
                cout << "\nPresione enter...";
                cin.ignore();
                cin.get();
                break;
                
            case 7:
                plano.mostrarGrupos();
                cout << "\nPresione enter...";
                cin.ignore();
                cin.get();
                break;
                
            case 8:
                cout << "Saliendo...\n";
                break;
                
            default:
                cout << "Opcion invalida. Presione enter...";
                cin.ignore();
                cin.get();
                break;
        }
    } while(opcion != 8);

    return 0;
}
// hallar disancia promedio entre los puntos al nuevo y elegir el grupo mas cercano al nuevo punto para asignarlo a ese grupo
// TODO: input(x,y,Nombre;Grupo)
// mostrar los grupos por coloresx
// ejemplo imc( indice de masa corporal) -> bajo peso, normal, sobrepeso, obesidad