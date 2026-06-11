# TSP OpenMP

Implementacion en C de un algoritmo genetico para resolver aproximaciones del Problema del Viajante (TSP). El proyecto permite ejecutar el algoritmo en modo maestro-esclavo o en modo islas usando OpenMP.

## Requisitos

- GCC
- OpenMP

## Compilacion

Desde la raiz del proyecto:

```bash
gcc -fopenmp src/main.c src/genetic.c src/tsp.c -lm -o src/main
```

## Ejecucion

Formato general:

```bash
./src/main <archivo.tsp> [maestro|islas] [hilos] [intervalo_migracion]
```

Ejemplos:

```bash
./src/main berlin52.tsp
./src/main kroA200.tsp maestro 4
./src/main pr1002.tsp islas 4 100
```

Parametros:

- `archivo.tsp`: instancia del problema en formato TSPLIB.
- `maestro|islas`: modo de ejecucion. Por defecto usa `maestro`.
- `hilos`: cantidad de hilos de OpenMP.
- `intervalo_migracion`: cada cuantas generaciones migran individuos en modo `islas`. Por defecto es `100`.

## Archivos

- `src/main.c`: lectura del archivo, configuracion y ciclo principal.
- `src/genetic.c`: operadores geneticos y versiones paralelas.
- `src/tsp.c`: calculo de distancias y rutas.
- `berlin52.tsp`, `kroA200.tsp`, `pr1002.tsp`: instancias de prueba.

## Notas

El programa imprime la poblacion inicial, las mejoras encontradas y la mejor ruta al finalizar. La simulacion ejecuta hasta 1.000.000 de generaciones.
