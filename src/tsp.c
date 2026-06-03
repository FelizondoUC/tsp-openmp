#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "tsp.h"

/*
    Calcula la distancia euclidiana
    entre dos ciudades.

    Fórmula:
    sqrt((x2-x1)^2 + (y2-y1)^2)
*/
double calcular_distancia(Ciudad a, Ciudad b) {

    double dx = a.x - b.x;
    double dy = a.y - b.y;

    return sqrt(dx * dx + dy * dy);
}

/*
    Calcula la distancia total recorrida
    por una ruta.

    También incluye el regreso
    a la ciudad inicial.
*/
double distancia_total(
    Ciudad *ciudades,
    int *ruta,
    int num_ciudades
) {

    double total = 0;

    /*
        Recorremos la ruta sumando
        la distancia entre ciudades consecutivas.
    */
    for (int i = 0; i < num_ciudades - 1; i++) {

        total += calcular_distancia(
            ciudades[ruta[i]],
            ciudades[ruta[i + 1]]
        );
    }

    /*
        Agregamos la distancia
        desde la última ciudad
        hacia la ciudad inicial.
    */
    total += calcular_distancia(
        ciudades[ruta[num_ciudades - 1]],
        ciudades[ruta[0]]
    );

    return total;
}

/*
    Genera ciudades aleatorias.

    Cada ciudad obtiene coordenadas
    aleatorias entre 0 y 99.
*/
void generar_ciudades(
    Ciudad *ciudades,
    int num_ciudades
) {

    for (int i = 0; i < num_ciudades; i++) {

        ciudades[i].x = rand() % 100;
        ciudades[i].y = rand() % 100;
    }
}

/*
    Genera una ruta aleatoria.

    Primero se cargan las ciudades
    en orden secuencial.

    Luego se mezclan usando intercambios aleatorios.
*/
void generar_ruta_aleatoria(
    int *ruta,
    int num_ciudades
) {

    /*
        Inicializar la ruta:
        0 1 2 3 4 ...
    */
    for (int i = 0; i < num_ciudades; i++) {
        ruta[i] = i;
    }

    /*
        Mezclar la ruta aleatoriamente.
    */
    for (int i = 0; i < num_ciudades; i++) {

        int j = rand() % num_ciudades;

        int temp = ruta[i];
        ruta[i] = ruta[j];
        ruta[j] = temp;
    }
}
