#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "tsp.h"
void generar_ruta_aleatoria(int *ruta, int num_ciudades) {
    // Paso 1: Llenar la ruta en orden secuencial (0, 1, 2, ..., N-1)
    for (int i = 0; i < num_ciudades; i++) {
        ruta[i] = i; 
    }

    // Paso 2: Barajado de Fisher-Yates
    for (int i = num_ciudades - 1; i > 0; i--) {
        // Elegir un índice aleatorio entre 0 e i
        int j = rand() % (i + 1);
        
        // Intercambiar ruta[i] con ruta[j]
        int temporal = ruta[i];
        ruta[i] = ruta[j];
        ruta[j] = temporal;
    }
}

double calcular_distancia(Ciudad a, Ciudad b) {
    double dx = (double)(a.coord_x - b.coord_x);
    double dy = (double)(a.coord_y - b.coord_y);
    return sqrt(dx * dx + dy * dy);
}

double distancia_total(Ciudad *ciudades, int *ruta, int num_ciudades) {
    double total = 0.0;
    for (int i = 0; i < num_ciudades - 1; i++) {
        total += calcular_distancia(ciudades[ruta[i]], ciudades[ruta[i+1]]);
    }
    // Volver a la ciudad de origen para cerrar el ciclo
    if (num_ciudades > 0) {
        total += calcular_distancia(ciudades[ruta[num_ciudades - 1]], ciudades[ruta[0]]);
    }
    return total;
}
