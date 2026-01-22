#include <iostream>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

// --- Función Quick Sort usando punteros ---
void quickSort(int* arr, int inicio, int fin) {
    int i = inicio, j = fin;
    int pivote = *(arr + (inicio + fin) / 2); // Acceso por puntero

    while (i <= j) {
        while (*(arr + i) < pivote) i++;
        while (*(arr + j) > pivote) j--;
        if (i <= j) {
            swap(*(arr + i), *(arr + j));
            i++;
            j--;
        }
    }
    if (inicio < j) quickSort(arr, inicio, j);
    if (i < fin) quickSort(arr, i, fin);
}

// --- Función Bubble Sort usando punteros ---
void bubbleSort(int* arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (*(arr + j) > *(arr + j + 1)) {
                swap(*(arr + j), *(arr + j + 1));
            }
        }
    }
}

int main() {
    srand(time(0));
    
    int n = 10000;           
    int incremento = 10000;  
    double limiteTiempo = 60.0; // Extendemos el límite a 60 segundos
    double tiempoBubble = 0;

    cout << fixed << setprecision(5);
    cout << "ESTRES DE CPU: Bubble Sort vs Quick Sort (Limite 60s)" << endl;
    cout << "============================================================" << endl;
    cout << "Elementos\t| Bubble Sort (seg)\t| Quick Sort (seg)" << endl;
    cout << "------------------------------------------------------------" << endl;

    while (tiempoBubble < limiteTiempo) {
        // Reservamos memoria dinámica usando punteros
        int* datosB = new int[n];
        int* datosQ = new int[n];

        // Llenar ambos con los mismos números aleatorios
        for (int i = 0; i < n; i++) {
            int num = rand() % 100000;
            *(datosB + i) = num;
            *(datosQ + i) = num;
        }

        // --- Prueba Bubble Sort ---
        clock_t tInicioB = clock();
        bubbleSort(datosB, n);
        clock_t tFinB = clock();
        tiempoBubble = double(tFinB - tInicioB) / CLOCKS_PER_SEC;

        // --- Prueba Quick Sort ---
        clock_t tInicioQ = clock();
        quickSort(datosQ, 0, n - 1);
        clock_t tFinQ = clock();
        double tiempoQuick = double(tFinQ - tInicioQ) / CLOCKS_PER_SEC;

        // Mostrar resultados
        cout << n << "\t\t| " << tiempoBubble << " s\t| " << tiempoQuick << " s" << endl;

        // Liberar la memoria para evitar que la laptop se quede sin RAM
        delete[] datosB;
        delete[] datosQ;

        // Aumentar la carga de trabajo
        n += incremento;
    }

    cout << "------------------------------------------------------------" << endl;
    cout << "PRUEBA FINALIZADA: Bubble Sort llego al limite de 60 segundos." << endl;
    
    system("pause");
    return 0;
}