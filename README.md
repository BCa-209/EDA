# **EDA - Estructura de Datos Avanzadas**
Repositorio académico con implementaciones en **C++** de distintas **estructuras de datos** y pruebas básicas de rendimiento.  
Pensado para prácticas universitarias y refuerzo de conceptos fundamentales.


## Temas incluidos

- **Test CPU**: Pruebas simples de rendimiento y uso de CPU.
    - Pruebas simples de rendimiento
    - Comparación de algoritmos de ordenamiento
- **Pilas y colas**: Ejemplos de uso de pilas y colas.
    - Implementaciones usando Programación Orientada a Objetos (POO)
- **Listas**: Implementaciones básicas de listas enlazadas.
    - Listas simplemente enlazadas
    - Ejemplos básicos de operaciones (insertar, eliminar, recorrer)
    - Ejemplos prácticos de uso
- **Agenda**: 
    - Implementación de una agenda usando estructuras de datos
    - Almacenamiento en archivos `.txt` sin cifrado


## Estructura
```text
EDA/
├── Listas/
│   ├── listas_enlazada.cpp              # Singly linked list
│   ├── listas_doble_enlazada.cpp        # Doubly linked list
│   └── listas_doble_enlazada_pila.cpp   # Stack/Queue adapters
├── pilas y colas/
│   ├── pila.cpp                         # Array-based stack
│   ├── cola.cpp                         # Array-based queue
│   └── cola_circular.cpp                # Circular queue
├── Agenda/
│   ├── test.cpp                         # Production contact manager
│   ├── interfaz.cpp                     # UI demonstration
│   ├── Agendav1.cpp                     # Demo business logic
│   ├── colors.h                         # ANSI color definitions
│   └── agenda.txt                       # Contact database
├── test cpu/
│   ├── test.cpp                         # Bubble vs Quick Sort
│   ├── test1.cpp                        # User-defined size test
│   └── test2.cpp                        # Iterative size test
├── arbol_binario.cpp                    # Binary search tree
├── README.md                            # Compilation guide
└── .gitignore                           # VCS exclusions
```
## Teconologias utilizdas
- Lenguaje: **C++**
- Paradigma: **Programación Orientada a Objetos**
- Entorno: **Consola**

## ▶️ Cómo compilar y ejecutar

Este proyecto está desarrollado en **C++** y se ejecuta desde **consola**.

### 🔧 Requisitos
- Compilador **g++** (GCC).
- Sistema operativo: Windows / Linux / macOS
- Terminal o consola

---

### 🧪 Compilar un archivo individual

Desde la raíz del proyecto:

```bash
g++ Agenda/test_pilas.cpp -o test_pilas
./test_pilas
```


## Test de CPU
Comparar el rendimiento de algortimos de ordenamiento.

***_Tabla de ejemplo de eficiencia del algoritmo_*** 

| Left columns  | Right columns |
| ------------- |:-------------:|
| left foo      | right foo     |
| left bar      | right bar     |
| left baz      | right baz     |


## Pilas y Colas 
Ejemplos de codigo para pilas y colas usando POO.
### Images

![This is an alt text.](/image/Markdown-mark.svg "This is a sample image.")


## Listas Enlazadas
Codigo de listas enlazadas.
### Images

![This is an alt text.](/image/Markdown-mark.svg "This is a sample image.")


## Agenda
Proyecto de una agenda guardada en formato`.txt` sin cifrar.
### Images

![This is an alt text.](/image/Markdown-mark.svg "This is a sample image.")

