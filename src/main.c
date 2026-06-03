#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tsp.h"
#include "genetic.h"

#define NUM_CIUDADES 20

int main() {

    srand(time(NULL));

    /*
        Crear ciudades.
    */
    Ciudad ciudades[NUM_CIUDADES];

    generar_ciudades(
        ciudades,
        NUM_CIUDADES
    );

    /*
        Crear población.
    */
    Individuo poblacion[TAM_POBLACION];

    generar_poblacion(
        poblacion,
        NUM_CIUDADES
    );

    /*
        Evaluar población.
    */
    evaluar_poblacion(
        poblacion,
        ciudades,
        NUM_CIUDADES
    );

    /*
        Obtener mejor individuo.
    */
    Individuo mejor =
        mejor_individuo(poblacion);

    /*
        Mostrar mejor resultado.
    */
    printf(
        "Mejor distancia inicial: %.2f\n",
        mejor.distancia
    );

    printf("Ruta:\n");

    for (int i = 0; i < NUM_CIUDADES; i++) {

        printf(
            "%d ",
            mejor.ruta[i]
        );
    }

    printf("\n");

    /*
        Liberar memoria.
    */
    liberar_poblacion(
        poblacion
    );

    return 0;
}
