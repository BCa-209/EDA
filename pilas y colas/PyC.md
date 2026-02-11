# Implementación de Pilas y Colas en C++

## 📋 Descripción General

Este repositorio contiene tres implementaciones de estructuras de datos lineales usando arreglos estáticos en C++:

1. **Pila** (Stack - LIFO) - `pila.cpp`
2. **Cola Lineal** (Queue - FIFO) - `cola.cpp`  
3. **Cola Circular** (Circular Queue) - `cola_circular.cpp`

---


# 1️⃣ PILA ESTÁTICA (`pila.cpp`)

## 🧱 Estructura de Clase

```cpp
class Pila {
    private:
    int elemento[TAM];  // Arreglo estático
    int indice;         // Índice del tope (-1 = vacía)
};
```

## Operaciones Implementadas

|Operación	|Descripción	|Complejidad
|:-|:-|:-:|
|push(valor)	|Agrega elemento al tope	|O(1)
|pop()	|Elimina elemento del tope	|O(1)
|estaVacia()|	Verifica si la pila está vacía	|O(1)
|estaLlena()	|Verifica si la pila está llena	|O(1)
|clean()	|Vacía completamente la pila	|O(1)
|imprimir()	|Muestra todos los elementos	|O(n)

## Características
- Implementación LIFO (Last In, First Out)
- Tamaño fijo definido por #define TAM 10
- Índice -1 representa pila vacía
- Operaciones en tiempo constante
- Sin método top() para ver el tope sin eliminar
- Sin manejo de excepciones
    
### Images
![This is an alt text.](/images/pila_static.png "This is a sample image.")

# COLA LINEAL (`cola.cpp`)

## 🧱 Estructura de Clase

```cpp
class Cola {
    private:
    int elemento[TAM];  // Arreglo estático
    int frente;         // Índice del frente (siempre 0)
    int final;         // Índice del final (-1 = vacía)
};
```
## Operaciones Implementadas

|Operación	|Descripción	|Complejidad
|:-|:-|:-:|
|encolar(valor)	|Agrega al final	|O(1)
|desencolar()	|Elimina del frente	|O(1)*
|estaVacia()	|Verifica si la cola está vacía	|O(1)
|estaLlena()	|Verifica si la cola está llena	|O(1)
|limpiar()	|Vacía completamente la cola	|O(1)
|imprimir()	|Muestra todos los elementos	|O(n)

## Características
- Implementación FIFO (First In, First Out)
- Tamaño fijo definido por #define TAM 10
- Frente siempre en 0

### Images
![This is an alt text.](/images/cola_static.png "This is a sample image.")


# COLA CIRCULAR (`cola_circular.cpp`)

## 🧱 Estructura de Clase

```cpp
class Cola {
    private:
    int elementos[MAX];  // Arreglo estático
    int frente;         // Índice del frente (-1 = vacía)
    int final;         // Índice del final (-1 = vacía)
};
```
## Operaciones Implementadas

|Operación	|Descripción	|Complejidad
|:-|:-|:-:|
|encolar(valor)	|Agrega al final	|O(1)|
|desencolar()	|Elimina del frente	|O(1)|
|estaVacia()	|Verifica si la cola está vacía	|O(1)|
|estaLlena()	|Verifica si la cola está llena	|O(1)|
|imprimir()	|Muestra todos los elementos	|O(n)|

## Características
- Implementación FIFO eficiente
- Reutilización de espacios
- Tamaño fijo definido por #define MAX 10
- Índices que "dan la vuelta" usando módulo (%)

