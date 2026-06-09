#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tsp.h"
#include "genetic.h"

/*
    Intercambia dos ciudades
    aleatoriamente.
*/
void mutacion(
    Individuo *individuo,
    int num_ciudades
) {

    double prob =
        (double) rand() / RAND_MAX;

    /*
        Aplicar mutación
        según probabilidad.
    */
    if (prob < TAXA_MUTACION) {

        int a = rand() % num_ciudades;
        int b = rand() % num_ciudades;

        int temp =
            individuo->ruta[a];

        individuo->ruta[a] =
            individuo->ruta[b];

        individuo->ruta[b] =
            temp;
    }
}

/*
    Crossover simple.

    Primera mitad:
    padre1

    Segunda mitad:
    padre2
*/
void crossover(
    Individuo *padre1,
    Individuo *padre2,
    Individuo *hijo,
    int num_ciudades
) {

    int mitad =
        num_ciudades / 2;

    int *usado =
        calloc(num_ciudades,
        sizeof(int));

    /*
        Copiar primera mitad
        del padre1.
    */
    for (int i = 0; i < mitad; i++) {

        hijo->ruta[i] =
            padre1->ruta[i];

        usado[hijo->ruta[i]] = 1;
    }

    /*
        Completar con ciudades
        del padre2.
    */
    int pos = mitad;

    for (int i = 0; i < num_ciudades; i++) {

        int cidade =
            padre2->ruta[i];

        if (!usado[cidade]) {

            hijo->ruta[pos++] =
                cidade;

            usado[cidade] = 1;
        }
    }

    free(usado);
}

/*
    Genera una nueva población
    usando crossover y mutación.
*/
void nueva_generacion(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo
) {

    int mejor_indice = 0;
    for (int i = 1; i < TAM_POBLACION; i++) {
        if (poblacion[i].distancia < poblacion[mejor_indice].distancia) {
            mejor_indice = i;
        }
    }

    /*
        Elitismo: conservar el mejor individuo
        de la generación anterior.
    */
    nueva_poblacion[0].ruta =
        malloc(num_ciudades * sizeof(int));

    for (int i = 0; i < num_ciudades; i++) {
        nueva_poblacion[0].ruta[i] =
            poblacion[mejor_indice].ruta[i];
    }

    nueva_poblacion[0].distancia =
        poblacion[mejor_indice].distancia;

    nueva_poblacion[0].fitness =
        poblacion[mejor_indice].fitness;

    for (int i = 1; i < TAM_POBLACION; i++) {

        /*
            Reservar memoria
            para el hijo.
        */
        nueva_poblacion[i].ruta =
            malloc(num_ciudades * sizeof(int));

        /*
            Seleccionar padres.
        */
        Individuo padre1 =
            torneo(
                poblacion,
                tam_torneo,
                TAM_POBLACION
            );

        Individuo padre2 =
            torneo(
                poblacion,
                tam_torneo,
                TAM_POBLACION
            );

        /*
            Crear hijo.
        */
        crossover(
            &padre1,
            &padre2,
            &nueva_poblacion[i],
            num_ciudades
        );

        /*
            Aplicar mutación.
        */
        mutacion(
            &nueva_poblacion[i],
            num_ciudades
        );

        /*
            Calcular fitness.
        */
        nueva_poblacion[i].distancia =
            distancia_total(
                ciudades,
                nueva_poblacion[i].ruta,
                num_ciudades
            );

        nueva_poblacion[i].fitness =
            100000.0 / nueva_poblacion[i].distancia;
    }
}

void sustituir_poblacion(
    Individuo *poblacion,
    Individuo *nueva_poblacion
) {

    for (int i = 0; i < TAM_POBLACION; i++) {
        free(poblacion[i].ruta);
        poblacion[i] = nueva_poblacion[i];
        nueva_poblacion[i].ruta = NULL;
    }
}
