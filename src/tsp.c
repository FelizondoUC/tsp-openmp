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
