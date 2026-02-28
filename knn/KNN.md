# Clasificador de Puntos 2D y Análisis de IMC

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)]()

Un conjunto de herramientas en C++ para visualización y clasificación de puntos en un plano cartesiano 2D, con una implementación especializada para el análisis del Índice de Masa Corporal (IMC).

## Tabla de Contenidos
- [Características Principales](#-características-principales)
- [Proyectos](#-proyectos)
  - [Planov2: Visualizador de Puntos Genérico](#planov2-visualizador-de-puntos-genérico)
  - [IMC Clasificador: Análisis de Masa Corporal](#imc-clasificador-análisis-de-masa-corporal)
- [Algoritmos Implementados](#-algoritmos-implementados)
- [Requisitos del Sistema](#-requisitos-del-sistema)
- [Instalación y Uso](#-instalación-y-uso)
- [Ejemplos de Visualización](#-ejemplos-de-visualización)
- [Estructura del Código](#-estructura-del-código)
- [Contribuciones](#-contribuciones)

## Características Principales

- **Visualización 2D con Colores**: Representación gráfica de puntos en consola usando códigos ANSI
- **Clasificación Automática**: Dos métodos de clasificación: distancia promedio y KNN
- **Múltiples Grupos**: Soporte para hasta 6 grupos con colores distintivos
- **Cálculo de Distancias**: Distancia euclidiana entre puntos
- **Interfaz Interactiva**: Menús amigables en la versión IMC
- **Multiplataforma**: Compatible con Windows y Linux

## Proyectos

### Planov2: Visualizador de Puntos Genérico

Sistema base para visualización y clasificación de puntos en un plano 2D.

**Características específicas:**
- Coordenadas personalizables (por defecto 20x10)
- Validación de puntos únicos (sin duplicados)
- Visualización de puntos con nombres y grupos
- Clasificación por distancia promedio

**Ejemplo de uso:**
```cpp
Plano plano;
plano.limites(20, 10);
plano.agregarPunto('A', 2, 3, 1);  // Grupo 1
plano.agregarPunto('B', 1, 5, 1);
plano.agregarPunto('W', 20, 10, 2); // Grupo 2
plano.draw();
```
### IMC_clasif: Hallar el Grupo al que Pertenece Una Muestra

Usamos el codigo de `planov2.cpp` para poder clasificar muestras sin tener que usar la tabla de Indice de Masa Corporal para integra una muestra a un grupo, el programa nos dará el grupo al que pertenece automaticamente. Una version de KNN simplificada para aprendizaje supervisado.

Usamos el **eje X** como la altura (cm.) y el **eje Y** como el peso (Kg.)

| IMC         | Categoría | Grupo |
| ----------- | --------- | ----- |
| < 18.5      | Bajo peso | 1     |
| 18.5 – 24.9 | Normal    | 2     |
| 25 – 29.9   | Sobrepeso | 3     |
| ≥ 30        | Obesidad  | 4     |
Tabla de IMC de referencia


## Diferencia Entre `planov2.cpp` y `IMC_clasif.cpp`

| Característica | planov2.cpp | IMC_clasif.cpp |
|-|-|-|
| Propósito	| Visualización genérica de puntos | Clasificación de IMC
| Múltiples puntos | No permitido | Permitido (conteo con *)
| Escala	| Directa | Dividida por 10
| Interfaz	| Código fijo en main | Menú interactivo
| Agrupación | Solo visual | Visual y por listado

## Funcionamiento de codigos

### `Planov2.cpp`
Ejemplo de uso

![Plano 2D.](/images/Clasificacion/planov2.png "")

### `IMC_clasif.cpp`
Menu Interactivo

![Dibujo manual.](/images/Clasificacion/clasi_menu.png "")

Introducir datos de una muestra 

![Dibujo manual.](/images/Clasificacion/clasi_input.png "")

Mostrar todos los puntos en una escala **x10** cada punto (`*3`, `*2`, etc) contiene varias muestras agrupadas para no afectar la visulaizacion del plano 2D.

![Dibujo manual.](/images/Clasificacion/clasi_draw.png "")

Lista de todos los puntos y sus grupos Respectivos

![Dibujo manual.](/images/Clasificacion/clasi_listar.png "")
