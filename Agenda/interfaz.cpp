#include <iostream>
#include "colors.h"

using namespace std;

// Constantes para combinaciones de colores (según la paleta)
#define MARCO_PRINCIPAL BG_BLUE << WHITE
#define TITULO_PRINCIPAL BG_BLUE << LBLUE
#define OPCION_NORMAL BG_GREEN << BLACK
#define OPCION_PELIGROSA BG_RED << WHITE
#define ENCABEZADO_TABLA BG_LGREEN << BLACK
#define DATOS_CONTACTO BG_BLUE << WHITE
#define MENSAJE_PROMPT BG_GRAY << GREEN
#define MENSAJE_ERROR BG_RED << WHITE
#define OPCION_SELECCIONADA BG_LBLUE << BLACK

// Función para limpiar pantalla (compatible con diferentes sistemas)
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Pausa y espera entrada del usuario
void pausar() {
    cout << RESET;
    cin.ignore();
    cin.get();
}

// Dibujar menú principal
void mostrarMenu() {
    limpiarPantalla();

    cout << MARCO_PRINCIPAL;
    cout << "+--------------------------------------+\n";
    cout << "|";
    cout << TITULO_PRINCIPAL;
    cout << "           AGENDA PERSONAL            ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";
    cout << "+--------------------------------------+\n";
    cout << "|                                      |\n";

    // OPCIONES NORMALES (verde con negro)
    cout << "|  ";
    cout << OPCION_NORMAL;
    cout << "[1] Agregar contacto ";
    cout << MARCO_PRINCIPAL;
    cout << "               |\n";

    cout << "|  ";
    cout << OPCION_NORMAL;
    cout << "[2] Ver contactos ";
    cout << MARCO_PRINCIPAL;
    cout << "                  |\n";

    cout << "|  ";
    cout << OPCION_NORMAL;
    cout << "[3] Buscar contacto ";
    cout << MARCO_PRINCIPAL;
    cout << "                |\n";

    cout << "|  ";
    cout << OPCION_NORMAL;
    cout << "[4] Eliminar contacto ";
    cout << MARCO_PRINCIPAL;
    cout << "              |\n";

    // OPCIÓN PELIGROSA (rojo con blanco)
    cout << "|  ";
    cout << OPCION_PELIGROSA;
    cout << "[0] Salir ";
    cout << MARCO_PRINCIPAL;
    cout << "                          |\n";

    cout << "|                                      |\n";
    cout << "+--------------------------------------+\n";
    
    // PROMPT DE ENTRADA (gris con verde)
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Seleccione una opcion: ";
    cout << MARCO_PRINCIPAL;
    cout << "             |\n";
    cout << "+--------------------------------------+\n";

    cout << RESET;
    cout << "   -> ";
}

// Mostrar pantalla de contacto con colores actualizados
void mostrarContactos() {
    limpiarPantalla();

    cout << MARCO_PRINCIPAL;
    
    // Marco superior con título
    cout << "+------------------------------------------------+\n";
    cout << "|";
    cout << TITULO_PRINCIPAL;
    cout << "                LISTA DE CONTACTOS              ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";
    cout << "+------------------------------------------------+\n";

    // ENCABEZADOS DE TABLA (verde claro con negro)
    cout << "|  ";
    cout << ENCABEZADO_TABLA;
    cout << "Nombre              | Telefono    | Email      ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";

    // Separador (azul con blanco)
    cout << "+---------------------+-------------+------------+\n";

    // DATOS DE CONTACTOS (azul con blanco)
    cout << "| ";
    cout << DATOS_CONTACTO;
    cout << "Juan Perez          | 5551234     | juan@mail  ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";

    cout << "| ";
    cout << DATOS_CONTACTO;
    cout << "Ana Lopez           | 5559876     | ana@mail   ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";

    cout << "| ";
    cout << DATOS_CONTACTO;
    cout << "Carlos Diaz         | 5554567     | carlos@mail";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";

    cout << "| ";
    cout << DATOS_CONTACTO;
    cout << "Maria Garcia        | 5557890     | maria@mail ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";

    cout << "| ";
    cout << DATOS_CONTACTO;
    cout << "Pedro Martinez      | 5552345     | pedro@mail ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";

    // Marco inferior
    cout << "+------------------------------------------------+\n";

    // MENSAJE DE PROMPT (gris con verde)
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Presione ENTER para volver al menu...         ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";

    cout << "+------------------------------------------------+\n";

    cout << RESET;
    pausar();
}

// Mostrar pantalla para agregar contacto
void mostrarAgregarContacto() {
    limpiarPantalla();

    cout << MARCO_PRINCIPAL;
    cout << "+--------------------------------------+\n";
    cout << "|";
    cout << TITULO_PRINCIPAL;
    cout << "          AGREGAR CONTACTO             ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";
    cout << "+--------------------------------------+\n";
    cout << "|                                      |\n";
    
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Nombre: ";
    cout << MARCO_PRINCIPAL;
    cout << "                               |\n";
    cout << "|                                      |\n";
    
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Telefono: ";
    cout << MARCO_PRINCIPAL;
    cout << "                             |\n";
    cout << "|                                      |\n";
    
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Email: ";
    cout << MARCO_PRINCIPAL;
    cout << "                                |\n";
    cout << "|                                      |\n";
    
    cout << "+--------------------------------------+\n";
    
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Presione ESC para cancelar o ENTER para guardar";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "+--------------------------------------+\n";

    cout << RESET;
    
    // Simulación de entrada (se implementaría en una versión real)
    cout << DATOS_CONTACTO << "   (Esta funcionalidad está en desarrollo)   " << RESET << endl;
    pausar();
}

// Mostrar mensaje de error
void mostrarError(const string& mensaje) {
    limpiarPantalla();
    
    cout << MARCO_PRINCIPAL;
    cout << "+--------------------------------------+\n";
    cout << "|";
    cout << MENSAJE_ERROR;
    cout << "               ¡ERROR!                ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";
    cout << "+--------------------------------------+\n";
    cout << "|                                      |\n";
    
    cout << "|  ";
    cout << MENSAJE_ERROR;
    cout << " " << mensaje;
    // Añadir espacios para alinear
    for (size_t i = mensaje.length(); i < 36; i++) cout << " ";
    cout << MARCO_PRINCIPAL << " |\n";
    
    cout << "|                                      |\n";
    cout << "+--------------------------------------+\n";
    
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Presione ENTER para continuar...      ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "+--------------------------------------+\n";

    cout << RESET;
    pausar();
}

// Mostrar mensaje de éxito
void mostrarExito(const string& mensaje) {
    limpiarPantalla();
    
    cout << MARCO_PRINCIPAL;
    cout << "+--------------------------------------+\n";
    cout << "|";
    cout << OPCION_NORMAL;
    cout << "              ¡EXITO!                 ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";
    cout << "+--------------------------------------+\n";
    cout << "|                                      |\n";
    
    cout << "|  ";
    cout << OPCION_NORMAL;
    cout << " " << mensaje;
    // Añadir espacios para alinear
    for (size_t i = mensaje.length(); i < 36; i++) cout << " ";
    cout << MARCO_PRINCIPAL << " |\n";
    
    cout << "|                                      |\n";
    cout << "+--------------------------------------+\n";
    
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Presione ENTER para continuar...      ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "+--------------------------------------+\n";

    cout << RESET;
    pausar();
}

// Demostración de la paleta de colores
void mostrarPaletaColores() {
    limpiarPantalla();
    
    cout << MARCO_PRINCIPAL;
    cout << "+--------------------------------------+\n";
    cout << "|";
    cout << TITULO_PRINCIPAL;
    cout << "         PALETA DE COLORES             ";
    cout << MARCO_PRINCIPAL;
    cout << "|\n";
    cout << "+--------------------------------------+\n";
    
    cout << "|                                      |\n";
    
    cout << "|  ";
    cout << TITULO_PRINCIPAL;
    cout << " Título principal - Azul/LBlue        ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "|  ";
    cout << OPCION_NORMAL;
    cout << " Opciones normales - Verde/Negro      ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "|  ";
    cout << OPCION_PELIGROSA;
    cout << " Opción peligrosa - Rojo/Blanco       ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "|  ";
    cout << ENCABEZADO_TABLA;
    cout << " Encabezados tabla - LGreen/Negro     ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "|  ";
    cout << DATOS_CONTACTO;
    cout << " Datos contacto - Azul/Blanco         ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << " Mensajes/prompts - Gris/Verde        ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "|  ";
    cout << MENSAJE_ERROR;
    cout << " Errores/alertas - Rojo/Blanco        ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "|  ";
    cout << OPCION_SELECCIONADA;
    cout << " Seleccionado menú - LBlue/Negro      ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "|                                      |\n";
    cout << "+--------------------------------------+\n";
    
    cout << "|  ";
    cout << MENSAJE_PROMPT;
    cout << "Presione ENTER para volver...         ";
    cout << MARCO_PRINCIPAL;
    cout << " |\n";
    
    cout << "+--------------------------------------+\n";

    cout << RESET;
    pausar();
}

int main() {
    int opcion;

    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1:
                mostrarAgregarContacto();
                // En una versión real aquí se capturarían los datos
                mostrarExito("Contacto agregado exitosamente");
                break;

            case 2:
                mostrarContactos();
                break;

            case 3:
                // Simulación de búsqueda
                limpiarPantalla();
                cout << MARCO_PRINCIPAL;
                cout << "+--------------------------------------+\n";
                cout << "|";
                cout << TITULO_PRINCIPAL;
                cout << "          BUSCAR CONTACTO              ";
                cout << MARCO_PRINCIPAL;
                cout << "|\n";
                cout << "+--------------------------------------+\n";
                cout << "|                                      |\n";
                cout << "|  ";
                cout << MENSAJE_PROMPT;
                cout << "Buscando 'Juan'...                    ";
                cout << MARCO_PRINCIPAL;
                cout << " |\n";
                cout << "|                                      |\n";
                cout << "+--------------------------------------+\n";
                cout << RESET;
                pausar();
                break;

            case 4:
                // Simulación de eliminación
                limpiarPantalla();
                mostrarError("¿Está seguro de eliminar el contacto?");
                // En una versión real aquí se confirmaría
                break;

            case 9: // Opción secreta para ver la paleta de colores
                mostrarPaletaColores();
                break;

            case 0:
                limpiarPantalla();
                cout << MARCO_PRINCIPAL;
                cout << "+--------------------------------------+\n";
                cout << "|";
                cout << OPCION_PELIGROSA;
                cout << "        SALIENDO DE LA AGENDA         ";
                cout << MARCO_PRINCIPAL;
                cout << "|\n";
                cout << "+--------------------------------------+\n";
                cout << "|                                      |\n";
                cout << "|  ";
                cout << MENSAJE_PROMPT;
                cout << "¡Gracias por usar la agenda!         ";
                cout << MARCO_PRINCIPAL;
                cout << " |\n";
                cout << "|                                      |\n";
                cout << "+--------------------------------------+\n";
                cout << RESET;
                break;

            default:
                mostrarError("Opción inválida. Intente nuevamente.");
        }

    } while (opcion != 0);

    return 0;
}