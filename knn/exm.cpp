#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

using namespace std;

// Estructura para representar un punto en el espacio
struct Point {
    vector<double> features; // Características del punto (atributos)
    int label;               // Etiqueta de la clase (para clasificación)
};

// Función para calcular la distancia Euclidiana entre dos puntos
double euclideanDistance(const Point& p1, const Point& p2) {
    double distance = 0.0;
    for (size_t i = 0; i < p1.features.size(); ++i) {
        distance += pow(p1.features[i] - p2.features[i], 2);
    }
    return sqrt(distance);
}

// Función que implementa KNN
int knn(const vector<Point>& trainingData, const Point& testPoint, int K) {
    // Calcular las distancias entre el punto de prueba y todos los puntos de entrenamiento
    vector<pair<double, int>> distances; // Par: (distancia, etiqueta)
    
    for (const auto& trainPoint : trainingData) {
        double dist = euclideanDistance(testPoint, trainPoint);
        distances.push_back({dist, trainPoint.label});
    }

    // Ordenar los puntos por distancia
    sort(distances.begin(), distances.end());

    // Contar las clases más frecuentes entre los K vecinos más cercanos
    map<int, int> frequencyMap;
    for (int i = 0; i < K; ++i) {
        frequencyMap[distances[i].second]++;
    }

    // Encontrar la clase con la mayor frecuencia
    int maxFreq = 0;
    int predictedLabel = -1;
    for (const auto& entry : frequencyMap) {
        if (entry.second > maxFreq) {
            maxFreq = entry.second;
            predictedLabel = entry.first;
        }
    }

    return predictedLabel;
}

int main() {
    // Datos de entrenamiento (características + etiquetas)
    vector<Point> trainingData = {
        {{1.0, 2.0}, 0},
        {{2.0, 3.0}, 0},
        {{3.0, 3.0}, 1},
        {{6.0, 5.0}, 1},
        {{7.0, 8.0}, 1},
        {{8.0, 8.0}, 1}
    };

    // Punto de prueba (con características)
    Point testPoint = {{5.0, 5.0}, -1}; // La etiqueta -1 significa que es un punto de prueba

    // Número de vecinos a considerar
    int K = 3;

    // Predecir la clase del punto de prueba
    int predictedClass = knn(trainingData, testPoint, K);

    cout << "La clase predicha para el punto de prueba es: " << predictedClass << endl;

    return 0;
}
