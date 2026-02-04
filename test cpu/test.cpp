#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

/* ---------- QUICK SORT ---------- */
int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/* ---------- MAIN ---------- */
int main() {
    const int N = 1'000'000;

    int* original = new int[N];
    int* bubbleArr = new int[N];
    int* quickArr  = new int[N];

    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        original[i] = rand();
        bubbleArr[i] = original[i];
        quickArr[i]  = original[i];
    }

    /* ================= BUBBLE SORT ================= */
    cout << "==== PRUEBA 1: BUBBLE SORT (1 MINUTO) ====\n";

    long long comparaciones = 0;
    int pasadas = 0;
    int elementosOrdenados = 0;

    auto inicioBubble = steady_clock::now();
    bool terminado = true;

    for (int i = 0; i < N - 1; i++) {
        bool swapped = false;

        for (int j = 0; j < N - i - 1; j++) {
            comparaciones++;

            if (bubbleArr[j] > bubbleArr[j + 1]) {
                swap(bubbleArr[j], bubbleArr[j + 1]);
                swapped = true;
            }

            auto ahora = steady_clock::now();
            if (duration<double>(ahora - inicioBubble).count() >= 60.0) {
                terminado = false;
                break;
            }
        }

        if (!terminado) break;

        pasadas++;
        elementosOrdenados = pasadas;

        if (!swapped) break;
    }


    auto finBubble = steady_clock::now();
    double tiempoBubble = duration<double>(finBubble - inicioBubble).count();

    cout << "Tiempo Bubble Sort: " << tiempoBubble << " segundos\n";
    cout << "Pasadas completadas: " << pasadas << endl;
    cout << "Comparaciones: " << comparaciones << endl;
    cout << "Elementos ordenados en 1 minuto: " << elementosOrdenados << " de " << N << endl;
    cout << "Estado: " << (terminado ? "COMPLETADO" : "DETENIDO POR LIMITE") << "\n\n";

    /* ================= QUICK SORT ================= */
    cout << "==== PRUEBA 2: QUICK SORT ====\n";

    auto inicioQuick = steady_clock::now();
    quickSort(quickArr, 0, N - 1);
    auto finQuick = steady_clock::now();

    double tiempoQuick = duration<double>(finQuick - inicioQuick).count();

    cout << "Tiempo QuickSort: " << tiempoQuick << " segundos\n";
    cout << "Estado: ORDENAMIENTO COMPLETADO\n";

    /* ================= CONCLUSION ================= */
    cout << "\n==== CONCLUSION ====\n";
    cout << "Bubble Sort es inviable para 1 millon de datos\n";
    cout << "QuickSort es eficiente y escala correctamente\n";

    delete[] original;
    delete[] bubbleArr;
    delete[] quickArr;

    system("pause");
    return 0;
}
