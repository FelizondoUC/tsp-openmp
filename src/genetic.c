#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "tsp.h"
#include "genetic.h"

static unsigned int rng_siguiente(unsigned int *semilla) {
    *semilla = (*semilla * 1103515245u + 12345u);
    return (*semilla / 65536u) % 32768u;
}

static int rng_entero(unsigned int *semilla, int limite) {
    return (int)(rng_siguiente(semilla) % (unsigned int)limite);
}

static double rng_double(unsigned int *semilla) {
    return (double)rng_siguiente(semilla) / 32767.0;
}

static unsigned int crear_semilla_hilo(int id_hilo) {
    static unsigned int contador_semillas = 0;
    unsigned int numero_semilla;

#ifdef _OPENMP
    #pragma omp atomic capture
#endif
    numero_semilla = contador_semillas++;

    return (unsigned int)time(NULL)
        ^ (unsigned int)clock()
        ^ (unsigned int)(id_hilo * 2654435761u)
        ^ (unsigned int)(numero_semilla * 1013904223u);
}

static Individuo torneo_con_semilla(
    Individuo *poblacion,
    int k,
    int tam_poblacion,
    unsigned int *semilla
) {
    int mejor_indice = rng_entero(semilla, tam_poblacion);

    for (int i = 1; i < k; i++) {
        int indice = rng_entero(semilla, tam_poblacion);

        if (poblacion[indice].distancia < poblacion[mejor_indice].distancia) {
            mejor_indice = indice;
        }
    }

    return poblacion[mejor_indice];
}

#ifdef _OPENMP
static Individuo torneo_rango_con_semilla(
    Individuo *poblacion,
    int k,
    int inicio,
    int fin,
    unsigned int *semilla
) {
    int tam_rango = fin - inicio;
    int mejor_indice = inicio + rng_entero(semilla, tam_rango);

    for (int i = 1; i < k; i++) {
        int indice = inicio + rng_entero(semilla, tam_rango);

        if (poblacion[indice].distancia < poblacion[mejor_indice].distancia) {
            mejor_indice = indice;
        }
    }

    return poblacion[mejor_indice];
}
#endif

static void mutacion_con_semilla(
    Individuo *individuo,
    int num_ciudades,
    unsigned int *semilla
) {
    double prob = rng_double(semilla);

    if (prob < TAXA_MUTACION) {
        int a = rng_entero(semilla, num_ciudades);
        int b = rng_entero(semilla, num_ciudades);

        int temp = individuo->ruta[a];

        individuo->ruta[a] = individuo->ruta[b];

        individuo->ruta[b] = temp;
    }
}

static void copiar_individuo(
    Individuo *destino,
    Individuo *origen,
    int num_ciudades
) {
    destino->ruta = malloc(num_ciudades * sizeof(int));

    for (int i = 0; i < num_ciudades; i++) {
        destino->ruta[i] = origen->ruta[i];
    }

    destino->distancia = origen->distancia;
    destino->fitness = origen->fitness;
}

static int buscar_mejor_indice(Individuo *poblacion) {
    int mejor_indice = 0;

    for (int i = 1; i < TAM_POBLACION; i++) {
        if (poblacion[i].distancia < poblacion[mejor_indice].distancia) {
            mejor_indice = i;
        }
    }

    return mejor_indice;
}

#ifdef _OPENMP
static int buscar_mejor_indice_rango(
    Individuo *poblacion,
    int inicio,
    int fin
) {
    int mejor_indice = inicio;

    for (int i = inicio + 1; i < fin; i++) {
        if (poblacion[i].distancia < poblacion[mejor_indice].distancia) {
            mejor_indice = i;
        }
    }

    return mejor_indice;
}

static void calcular_rango_isla(
    int id_isla,
    int cantidad_islas,
    int *inicio,
    int *fin
) {
    int base = TAM_POBLACION / cantidad_islas;
    int resto = TAM_POBLACION % cantidad_islas;

    *inicio = id_isla * base;

    if (id_isla < resto) {
        *inicio += id_isla;
        *fin = *inicio + base + 1;
    } else {
        *inicio += resto;
        *fin = *inicio + base;
    }
}
#endif

static void generar_hijo(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo,
    int indice_hijo,
    unsigned int *semilla
) {
    nueva_poblacion[indice_hijo].ruta = malloc(num_ciudades * sizeof(int));

    Individuo padre1 =
        torneo_con_semilla(
            poblacion,
            tam_torneo,
            TAM_POBLACION,
            semilla
        );

    Individuo padre2 =
        torneo_con_semilla(
            poblacion,
            tam_torneo,
            TAM_POBLACION,
            semilla
        );

    crossover(
        &padre1,
        &padre2,
        &nueva_poblacion[indice_hijo],
        num_ciudades
    );

    mutacion_con_semilla(
        &nueva_poblacion[indice_hijo],
        num_ciudades,
        semilla
    );

    nueva_poblacion[indice_hijo].distancia = distancia_total(
            ciudades,
            nueva_poblacion[indice_hijo].ruta,
            num_ciudades
        );

    nueva_poblacion[indice_hijo].fitness =
        100000.0 / nueva_poblacion[indice_hijo].distancia;
}

#ifdef _OPENMP
static void generar_hijo_en_rango(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo,
    int indice_hijo,
    int inicio,
    int fin,
    unsigned int *semilla
) {
    nueva_poblacion[indice_hijo].ruta = malloc(num_ciudades * sizeof(int));

    Individuo padre1 =
        torneo_rango_con_semilla(
            poblacion,
            tam_torneo,
            inicio,
            fin,
            semilla
        );

    Individuo padre2 =
        torneo_rango_con_semilla(
            poblacion,
            tam_torneo,
            inicio,
            fin,
            semilla
        );

    crossover(
        &padre1,
        &padre2,
        &nueva_poblacion[indice_hijo],
        num_ciudades
    );

    mutacion_con_semilla(
        &nueva_poblacion[indice_hijo],
        num_ciudades,
        semilla
    );

    nueva_poblacion[indice_hijo].distancia = distancia_total(
            ciudades,
            nueva_poblacion[indice_hijo].ruta,
            num_ciudades
        );

    nueva_poblacion[indice_hijo].fitness =
        100000.0 / nueva_poblacion[indice_hijo].distancia;
}

static void reemplazar_individuo(
    Individuo *destino,
    Individuo *origen,
    int num_ciudades
) {
    if (destino == origen) {
        return;
    }

    free(destino->ruta);
    copiar_individuo(destino, origen, num_ciudades);
}
#endif

/*
    Intercambia dos ciudades
    aleatoriamente.
*/
void mutacion(
    Individuo *individuo,
    int num_ciudades
) 
{

    double prob = (double) rand() / RAND_MAX; //esto va a devolver un 
    //numero entre 0 y 1.0

    /*
        Aplicar mutación
        según probabilidad.
    */
    if (prob < TAXA_MUTACION) {

        int a = rand() % num_ciudades;
        int b = rand() % num_ciudades; //si fue afortunado de conseguir mutacion
        //entonces elegimos que indice

        int temp = individuo->ruta[a];

        individuo->ruta[a] = individuo->ruta[b];

        individuo->ruta[b] = temp;
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
) 
{

    int mitad = num_ciudades / 2;

    int *usado =calloc(num_ciudades, sizeof(int));

    /*
        Copiar primera mitad
        del padre1.
    */
    for (int i = 0; i < mitad; i++) {

        hijo->ruta[i] = padre1->ruta[i];

        usado[hijo->ruta[i]] = 1;
    }

    /*
        Completar con ciudades
        del padre2.
    */
    int pos = mitad;

    for (int i = 0; i < num_ciudades; i++) {

        int cidade = padre2->ruta[i];

        if (!usado[cidade]) {

            hijo->ruta[pos++] = cidade;

            usado[cidade] = 1;
        }
    }

    free(usado);
}

/*
    Genera una nueva población
    
*/
void nueva_generacion_secuencial(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo
) {

    int mejor_indice = buscar_mejor_indice(poblacion);

    copiar_individuo(
        &nueva_poblacion[0],
        &poblacion[mejor_indice],
        num_ciudades
    );

    unsigned int semilla = crear_semilla_hilo(0);

    for (int i = 1; i < TAM_POBLACION; i++) {
        generar_hijo(
            poblacion,
            nueva_poblacion,
            ciudades,
            num_ciudades,
            tam_torneo,
            i,
            &semilla
        );
    }
}

/*
    Genera una nueva población con modelo maestro-esclavo.

    El hilo maestro conserva el mejor individuo y coordina la cola
    de trabajo. Los hilos esclavos toman bloques de hijos de forma
    dinámica para balancear la carga cuando algunos cruces/mutaciones
    tarden más que otros.
*/
void nueva_generacion_maestro_esclavo(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo
) {
#ifndef _OPENMP
    nueva_generacion_secuencial(
        poblacion,
        nueva_poblacion,
        ciudades,
        num_ciudades,
        tam_torneo
    );
#else
    int mejor_indice = buscar_mejor_indice(poblacion);

    #pragma omp parallel
    {
        int id_hilo = omp_get_thread_num();
        unsigned int semilla = crear_semilla_hilo(id_hilo);

        if (id_hilo == 0) {
            copiar_individuo(
                &nueva_poblacion[0],
                &poblacion[mejor_indice],
                num_ciudades
            );
        }

        #pragma omp barrier
        
        #pragma omp for schedule(dynamic, 1)
        for (int i = 1; i < TAM_POBLACION; i++) {
            generar_hijo(
                poblacion,
                nueva_poblacion,
                ciudades,
                num_ciudades,
                tam_torneo,
                i,
                &semilla
            );
        }

    }
#endif
}

/*
    Genera una nueva población con modelo de islas.

    Cada hilo trabaja sobre una partición de la población. Los torneos
    se hacen dentro de la misma isla y cada cierto intervalo se migra
    el mejor individuo global hacia el inicio de cada isla.
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
) {
    if (cantidad_islas < 1) {
        cantidad_islas = 1;
    }

    if (cantidad_islas > TAM_POBLACION) {
        cantidad_islas = TAM_POBLACION;
    }

#ifndef _OPENMP
    (void)generacion;
    (void)intervalo_migracion;

    nueva_generacion_secuencial(
        poblacion,
        nueva_poblacion,
        ciudades,
        num_ciudades,
        tam_torneo
    );
#else
    #pragma omp parallel num_threads(cantidad_islas)
    {
        int id_isla = omp_get_thread_num();
        int inicio;
        int fin;
        unsigned int semilla = crear_semilla_hilo(id_isla);

        calcular_rango_isla(
            id_isla,
            cantidad_islas,
            &inicio,
            &fin
        );

        int mejor_local = buscar_mejor_indice_rango(
            poblacion,
            inicio,
            fin
        );

        copiar_individuo(
            &nueva_poblacion[inicio],
            &poblacion[mejor_local],
            num_ciudades
        );

        for (int i = inicio + 1; i < fin; i++) {
            generar_hijo_en_rango(
                poblacion,
                nueva_poblacion,
                ciudades,
                num_ciudades,
                tam_torneo,
                i,
                inicio,
                fin,
                &semilla
            );
        }
    }

    if (
        intervalo_migracion > 0 &&
        generacion > 0 &&
        generacion % intervalo_migracion == 0
    ) {
        int mejor_global = buscar_mejor_indice(nueva_poblacion); //mirar bien complejidad

        for (int id_isla = 0; id_isla < cantidad_islas; id_isla++) {
            int inicio;
            int fin;

            calcular_rango_isla(
                id_isla,
                cantidad_islas,
                &inicio,
                &fin
            );

            (void)fin;
            reemplazar_individuo(
                &nueva_poblacion[inicio],
                &nueva_poblacion[mejor_global],
                num_ciudades
            );
        }
    }
#endif
}

void nueva_generacion(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades,
    int tam_torneo
) {
    nueva_generacion_maestro_esclavo(
        poblacion,
        nueva_poblacion,
        ciudades,
        num_ciudades,
        tam_torneo
    );
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
