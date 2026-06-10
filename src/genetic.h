#ifndef GENETIC_H
#define GENETIC_H

#include "tsp.h"

#define TAM_POBLACION 20
#define TAXA_MUTACION 0.1
#define TAM_TORNEO 5

/*
    Realiza mutación
    intercambiando ciudades.
*/
void mutacion(
    Individuo *individuo,
    int num_ciudades
);

/*
    Realiza crossover
    entre dos padres.
*/
void crossover(
    Individuo *padre1,
    Individuo *padre2,
    Individuo *hijo,
    int num_ciudades
);

/*
    Genera una nueva población secuencial.
*/
void nueva_generacion_secuencial(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo
);

/*
    Genera una nueva población paralela
    con modelo maestro-esclavo.
*/
void nueva_generacion_maestro_esclavo(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo
);

/*
    Genera una nueva población paralela
    con modelo de islas.
*/
void nueva_generacion_islas(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo,
    int cantidad_islas,
    int generacion,
    int intervalo_migracion
);

/*
    Genera una nueva población.
    Por defecto usa la versión paralela
    maestro-esclavo si se compila con OpenMP.
*/
void nueva_generacion(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo
);

/*
    Sustituye la población anterior
    por la nueva población generada.
*/
void sustituir_poblacion(
    Individuo *poblacion,
    Individuo *nueva_poblacion
);

#endif
