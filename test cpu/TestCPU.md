# TestCPU

Esta carpeta contiene un conjunto de pruebas de estrés (benchmarks) diseñadas para evaluar el rendimiento de la Unidad Central de Procesamiento (CPU). Las pruebas se basan en la comparación de eficiencia entre dos algoritmos de ordenamiento con distinta complejidad computacional: **Bubble Sort** ($O(n^2)$) y **Quick Sort** ($O(n \log n)$).

El objetivo es visualizar cómo la CPU maneja cargas de trabajo exponenciales versus logarítmicas y determinar los límites prácticos de procesamiento en un tiempo determinado.

## Descripcion de Archivos

### Archivo: `test.cpp`
Tiene como obejitivo demostrar la inviabilidad de Bubble Sort con grandes volúmenes de datos ($1,000,000$) y compararlo con la eficiencia de Quick Sort.

#### Características Principales

- **Vlumen de datos**: Fija una cte `N=1000000`
- **Mecanismo de Seguridad**: Implementa un "Time-Out" dentro del bucle de Bubble Sort. Si la ejecución supera 60 seg. rompe el bucle forzadamente.
- **Medicion de Tiempo**: Utiliza la libreria moderna  `<chrono>` (`steady_lock`) par aalta precicion.
- **Sintaxis**: Utiliza notacion de arreglos estandar (`arr[i]`) 
- **Metricas**: Cuenta comparaciones, pasad y elementos ordenados hasta el moemnto del corte.

#### Observaciones Técnicas
- Es el único archivo que intenta ejecutar Bubble Sort con 1 millón de datos, lo cual teóricamente tomaría horas o días sin el mecanismo de interrupción de 60 segundos.

- Genera tres arreglos: uno original y dos copias para asegurar que ambos algoritmos ordenen exactamente los mismos datos desordenados.

![This is an alt text.](/images/test1.png "This is a sample image.")

### Archivo: `test1.cpp`
Prueba unitaria interactiva enfocada exclusivamente en el algoritmo Bubble Sort utilizando aritmética de punteros.

#### Caracteristicas Principales
- **Interacción**: Solicita al usuario la cantidad de elementos (`cin >> n`).

- **Sintaxis de Punteros**: Implementa Bubble Sort utilizando explícitamente aritmética de punteros (`*(arr + j)`) en lugar de índices de arreglo.

- **Optimización**: Incluye la bandera `swapped` para detener el algoritmo si el arreglo ya está ordenado antes de terminar todas las pasadas.

- **Evaluación**: Clasifica el resultado como `[OK]` o `[X]` dependiendo de si tarda más o menos de 60 segundos.

#### Observaciones Tecnicas
- El código es más "educativo" respecto al manejo de memoria y punteros en `C++`.

- La lógica de intercambio (swap) se hace manualmente con una variable temporal temp, a diferencia de los otros archivos que usan la función std::swap.


### Archivo: `test2.cpp`
Realizar un "Stress Test" (Prueba de estrés) progresivo para generar una tabla comparativa de tiempos de ejecución.

#### Caracteristicas Principales
- **Metodología Incremental**: Comienza con 10,000 datos e incrementa de 10,000 en 10,000 en cada iteración.

- **Condición de Parada**: El programa continúa ejecutándose indefinidamente aumentando la carga hasta que Bubble Sort tarda más de 60 segundos en una sola iteración.

- **Gestión de Memoria**: Realiza asignación (`new`) y liberación (`delete[]`) de memoria dinámica dentro del bucle `while`. Esto es crucial para evitar desbordamiento de memoria (Memory Leaks) durante ejecuciones largas.

- **Sintaxis**: Utiliza aritmética de punteros tanto para Bubble Sort como para Quick Sort.

- **Librería de Tiempo**: Utiliza `<ctime>` y `clock()`, que es el estándar clásico de C (menos preciso que `hrono` pero funcional para este propósito).
#### Observaciones Tecnicas
- Es el script más completo para visualización de datos, ya que imprime una tabla formateada (`\t`) consola.
- Demuestra gráficamente cómo el tiempo de Bubble Sort crece exponencialmente ($O(n^2)$) mientras Quick Sort se mantiene casi instantáneo.

***_Tabla de ejemplo de eficiencia del algoritmo_*** 

| Elementos     | Bubble Sort (seg) | Quick Sort (seg) |
|:-------------:|:-------------:|:----------------:|
| 10000         | 0.21800 s     | 0.00000 s     |
| 20000         | 0.72100 s     | 0.00100 s     |
| 30000         | 1.67700 s     | 0.00200 s     |
| 40000         | 3.09900 s     | 0.00300 s     |
| 50000         | 6.06700 s     | 0.00500 s     |
| 60000         | 7.15600 s     | 0.00500 s     |
| 70000         | 9.56600 s     | 0.00700 s     |
| 80000         | 13.19100 s    | 0.00700 s     |
| 90000         | 18.63600 s    | 0.01000 s     |
| 100000        | 22.07100 s    | 0.01100 s     |
| 110000        | 26.64500 s    | 0.01000 s     |
| 120000        | 37.63300 s    | 0.01500 s     |
| 130000        | 44.37300 s    | 0.01500 s     |
| 140000        | 53.33200 s    | 0.02000 s     |
| 150000        | 61.93800 s    | 0.01500 s     |

![This is an alt text.](/images/test2.png "This is a sample image.")
