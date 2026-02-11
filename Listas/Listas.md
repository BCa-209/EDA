# Implementación de Listas Enlazadas en C++

## Descripción General

Este repositorio contiene tres implementaciones de listas enlazadas en C++:
1. **Lista simplemente enlazada**
2. **Lista doblemente enlazada**
3. **Lista doblemente enlazada con implementación de Pila y Cola**

## Estructura de Archivos
```
Listas/
├── listas_enlazada.cpp # Lista simplemente enlazada
├── listas_doble_enlazada.cpp # Lista doblemente enlazada
└── listas_doble_enlazada_pila.cpp # Lista doble + Pila/Cola
```
---

# LISTA SIMPLEMENTE ENLAZADA (`listas_enlazada.cpp`)

## Estructura de Clases

### `class Nodo`
```cpp
class Nodo {
    int dato;        // Valor almacenado
    Nodo* siguiente; // Puntero al siguiente nodo
};
```

### `Class Lista`
```cpp
class Lista {
    Nodo* cabezaptr; // Puntero al primer nodo
};
```

***_Tabla de ejemplo de eficiencia del algoritmo_*** 

|Operación	        |Descripción	                    |Complejidad    |
|:-|:--|:-:|
|addIni(valor)	    |Agrega al inicio	            |O(1)               |
|add(valor)	        |Agrega al final	                |O(n)           |
|addPos(valor, pos)	|Agrega en posición específica	|O(n)               |
|dropIni()	        |Elimina el primer nodo	        |O(1)               |
|drop()	            |Elimina el último nodo	        |O(n)               |
|dropPos(pos)	    |Elimina en posición específica  |O(n)              |
|print()	        | Muestra la lista	            |O(n)               |

## Caracteristicas

- Inserción al inicio, final y por posición
- Eliminación al inicio, final y por posición 
- Manejo de memoria con new/delete
- Destructor para liberación de memoria 
- No tiene puntero al último nodo


# LISTA DOBLEMENTE ENLAZADA (`listas_doble_enlazada.cpp`)

## Estructura de Clases

### `class Nodo`
```cpp
class Nodo {
    int dato;        // Valor almacenado
    Nodo* anterior;  // Puntero al nodo anterior
    Nodo* siguiente; // Puntero al siguiente nodo
};
```

### `Class Lista`
```cpp
class Lista {
    Nodo* cabezaptr; // Puntero al primer nodo
    Nodo* pieptr;    // Puntero al último nodo (declarado pero no usado)
};
```

***_Tabla de ejemplo de eficiencia del algoritmo_*** 

|Operación	        |Descripción    |Complejidad    |
|:-|:--|:-:|
|addIni(valor)	    |Agrega al inicio	    |O(1)
|add(valor)	        |Agrega al final	        |O(n)
|addPos(valor, pos)	|Agrega en posición	    |O(n)
|dropIni()	        |Elimina el primer nodo	|O(1)
|drop()          	|Elimina el último nodo	|O(n)
|dropPos(pos)	    |Elimina en posición	    |O(n)
|print()	            |Imprime normal (→)	    |O(n)
|printRev()	        |Imprime reversa (←)	    |O(n)

## Caracteristicas

- Punteros dobles (anterior y siguiente)
- Impresión en orden normal y reverso
- Manejo correcto de enlaces en inserciones/eliminaciones
- pieptr declarado pero no implementado
- Recorrido O(n) para llegar al final


# LISTA DOBLE + PILA/COLA (`listas_doble_enlazada_pila.cpp`)

## Estructura de Clases Adicionales  

### `class Pila`(LIFO)
```cpp
class Pila {
    Lista elementos; // Usa Lista doblemente enlazada
};
```

|Método	|Descripción|
|:-|:-|
|enpilar(valor)	|Agrega al inicio (push)|
|despilar()	|Elimina del inicio (pop)|
|mostrarPila()	|Muestra la pila|


### `Class Cola` (FIFO)
```cpp
class Cola {
    Lista elementos; // Usa Lista doblemente enlazada
};
```

|Método |	Descripción |
|:-|:-|
|encolar(valor)	|Agrega al final (enqueue)
|desencolar()	|Elimina del inicio (dequeue)
|mostrarCola()	|Muestra la cola


## Caracteristicas

- Pila implementada con addIni/dropIni
- Cola implementada con add/dropIni
- Reutilización de la clase Lista
- Operaciones O(1) en pila
- Cola con add() O(n) por falta de puntero al final



## EJEMPLOS DE USO

### Lista Simplemente Enlazada
```cpp
Lista lista;
lista.add(10);     // 10 -> NULL
lista.addIni(5);   // 5 -> 10 -> NULL
lista.addPos(7,1); // 5 -> 7 -> 10 -> NULL
lista.print();     // 5 -> 7 -> 10 -> NULL
```
### Images
![This is an alt text.](/images/lista_simp.png "This is a sample image.")

### Pila (LIFO)
```cpp
Pila pila;
pila.enpilar(1);
pila.enpilar(2);
pila.enpilar(3);   // 3 -> 2 -> 1
pila.despilar();    // 2 -> 1
pila.mostrarPila();
```
### Images
![This is an alt text.](/images/pila.png "This is a sample image.")

### Cola (FIFO)
```cpp
Cola cola;
cola.encolar(1);
cola.encolar(2);
cola.encolar(3);    // 1 -> 2 -> 3
cola.desencolar();  // 2 -> 3
cola.mostrarCola();
```
### Images
![This is an alt text.](/images/cola.png "This is a sample image.")