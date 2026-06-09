#ifndef TSP_H
#define TSP_H

/*
    Estructura que representa una ciudad.
    Cada ciudad tiene coordenadas x e y.
*/
typedef struct {
    int id_ciudad;
    float coord_x;
    float coord_y;
} Ciudad;

/*
    Estructura que representa un individuo
    del algoritmo genético.

    ruta:
        orden en el que se visitan las ciudades.

    distancia:
        distancia total recorrida en esa ruta.
*/
typedef struct {
    int *ruta;
    double distancia;
    float fitness;
} Individuo;

/*
    Calcula la distancia euclidiana
    entre dos ciudades.
*/
double calcular_distancia(Ciudad a, Ciudad b);

/*
    Calcula la distancia total de una ruta.

    ciudades:
        arreglo con todas las ciudades.

    ruta:
        orden de recorrido.

    num_ciudades:
        cantidad total de ciudades.
*/
double distancia_total(
    Ciudad *ciudades,
    int *ruta,
    int num_ciudades
);

/*
    Genera ciudades aleatorias.

    Cada ciudad tendrá coordenadas
    entre 0 y 99.
*/
void generar_ciudades(
    Ciudad *ciudades,
    int num_ciudades
);

/*
    Genera una ruta aleatoria.

    La ruta será una permutación
    de las ciudades.
*/
void generar_ruta_aleatoria(
    int *ruta,
    int num_ciudades
);

/*
    Selecciona un individuo usando torneo.

    Se sortean k individuos de la población
    y gana el que tenga menor distancia total.
*/
Individuo torneo(
    Individuo *poblacion,
    int k,
    int tam_poblacion
);

#endif
