# SISTEMA DE AGENDA EN C++

## Vision General

### Resumen
|Característica	|Descripción
|:-|:-|
|Propósito	|Agenda telefónica con árbol genealógico
|Estructura	|Lista doblemente enlazada
|Persistencia	|Archivo texto formato txt, vCard
|IDs	|Auto-incrementales basados en contador estático
|Relaciones	|Padre/Madre referenciados por ID
|UI	|Interfaz de consola con colores ANSI

### Funcionalidades
- **CRUD** completo de contactos
- Persistencia en archivo
- Búsqueda por nombre (parcial)
- Relaciones familiares (padres/hijos)
- Guardado automatico en `.txt`
- Exportación a `.vCF`
- IDs auto-incrementales

## Estructura de Archivos
```text
Agendav1.cpp
    ├── colors.h (incluido en test.cpp)
    ├── interfaz.cpp (versión standalone UI)
    ├── test.cpp (versión completa con UI)
    └── agenda.txt (datos persistidos)

Relación entre archivos:
├── Agendav1.cpp      → Versión funcional sin colores
├── test.cpp         → Versión completa con interfaz coloreada
├── interfaz.cpp     → Demostración de UI independiente
├── colors.h         → Definiciones de colores ANSI
└── agenda.txt       → Datos en formato vCard
```

## Analisis exclusivo de la versión completa (`test.cpp`)
`test.cpp` es la versión más completa y refinada del sistema de agenda. Combina la lógica funcional de `Agendav1.cpp` con la interfaz coloreada de `interfaz.cpp`, resultando en una aplicación de consola agradabale para el usuario.


### Dashboard
![This is an alt text.](/images/agenda/dash.png "This is a sample image.")

### Mostrar todos los Regstros
![This is an alt text.](/images/agenda/mostrar.png "This is a sample image.")

### Buscar por Nombre
![This is an alt text.](/images/agenda/buscar.png "This is a sample image.")

### Resultado de Busqueda
![This is an alt text.](/images/agenda/result_busqueda.png "This is a sample image.")

### Rwsultado ver FAmilia
![This is an alt text.](/images/agenda/familia.png "This is a sample image.")

### Guardado en Automatico (`.txt`)
```text
BEGIN:VCARD
VERSION:2.1
ID:1
N:Calderon;Brayan;Calderon2;;;
FN:Brayan Calderon Calderon2
TEL;CELL:972253527
TEL;CELL:960096116
EMAIL:brayan@mail.com
ADR:jr. mi jaus 123
PADRE_ID:
MADRE_ID:
END:VCARD
```

### Exprtado en Formato vCard (`.vcf`)
```text
BEGIN:VCARD
VERSION:2.1
N:Calderon;Brayan;Calderon2;;;
FN:Brayan Calderon
TEL;CELL:972253527
TEL;CELL:960096116
EMAIL:brayan@mail.com
ADR:jr. mi jaus 123
END:VCARD
```
