#include <iostream> 
#include <cstdlib> 
#include <ctime> 
#include <chrono> 
using namespace std; 
using namespace chrono; // Bubble Sort usando punteros 
void bubbleSort(int* arr, int n) { 
    for (int i = 0; i < n - 1; i++) { 
        bool swapped = false; 
        for (int j = 0; j < n - i - 1; j++) { 
            if (*(arr + j) > *(arr + j + 1)) { 
                int temp = *(arr + j);
                *(arr + j) = *(arr + j + 1); 
                *(arr + j + 1) = temp; 
                swapped = true; 
            } 
        } 
        if (!swapped) break; // Optimización 
        } 
    } 
int main() { 
    int n; 
    cout << "Ingrese la cantidad de elementos: "; 
    cin >> n; 
    // Reservar memoria dinámica 
    int* arr = new int[n]; // Llenar con valores aleatorios 
    srand(time(NULL)); 
    for (int i = 0; i < n; i++) { 
        *(arr + i) = rand(); 
    } 
    cout << "Ordenando..." << endl; 
    auto inicio = high_resolution_clock::now(); 
    bubbleSort(arr, n); 
    auto fin = high_resolution_clock::now(); 
    duration<double> tiempo = fin - inicio; 
    cout << "Tiempo de ejecucion: " << tiempo.count() << " segundos" << endl; 
    if (tiempo.count() >= 60.0) 
        cout << "[X] Supero 1 minuto" << endl; 
    
    else 
        cout << "[OK] Menos de 1 minuto" << endl;
    
    // Liberar memoria 
    delete[] arr; 
    system("pause");
    return 0; 
}