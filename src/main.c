#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tsp.h"
#include "genetic.h"


int main(int argc, char *argv[]) {

    int id_ciudad;
    float coord_x, coord_y;

    if (argc < 2) {
        printf("Uso: %s <nombre_del_archivo>\n", argv[0]);
        return 1; // Salir con error
    }

    char *nombre_archivo = argv[1];
    printf("Abriendo el archivo: %s\n", nombre_archivo);

    FILE *archivo;
    archivo = fopen(nombre_archivo, "r");

    // 2. Verificar si existe
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo.\n");
        return 1;
    }

    // Ignorar las primeras 6 líneas
    char buffer[256]; //tamaño maximo por linea
    for (int i = 0; i < 6; i++) {
        if (fgets(buffer, sizeof(buffer), archivo) == NULL) {
            break;
        }
    }

    int capacidad = 100;
    int num_ciudades = 0;

    // Reservamos memoria para 100 ciudades inicialmente
    Ciudad *ciudades = malloc(capacidad * sizeof(Ciudad));

    while (fscanf(archivo, "%d %f %f", &id_ciudad, &coord_x, &coord_y) == 3){
        if (num_ciudades >= capacidad) {
        capacidad *= 2;
        ciudades = realloc(ciudades, capacidad * sizeof(Ciudad));
    }
    
        // Guardamos en la posición actual
        ciudades[num_ciudades].id_ciudad = id_ciudad;
        ciudades[num_ciudades].coord_x = coord_x;
        ciudades[num_ciudades].coord_y = coord_y;
    
        num_ciudades++;
    }

    for (int i = 0; i < num_ciudades; i++) {
    printf("Posición arreglo [%d] -> ID: %d, X: %.2f, Y: %.2f\n", 
            i, 
            ciudades[i].id_ciudad, 
            ciudades[i].coord_x, 
            ciudades[i].coord_y);
}
    
int tam_poblacion = 20;

Individuo *poblacion = malloc(tam_poblacion * sizeof(Individuo));

for (int i = 0; i < tam_poblacion; i++) {
    // Para cada individuo, reservamos memoria para su ruta de ciudades
    poblacion[i].ruta = malloc(num_ciudades * sizeof(int));
    
    // Le asignamos una combinación aleatoria única
    generar_ruta_aleatoria(poblacion[i].ruta, num_ciudades);
    
    // Inicializamos su fitness en 0 (lo calcularás después sumando las distancias)
    poblacion[i].fitness = 0.0; 
}



for (int i = 0; i < tam_poblacion; i++) {
    printf("Ruta numero %d: ", i);
    
    for (int j = 0; j < num_ciudades; j++) {
        // Agregamos un espacio "%d " para separar los números de las ciudades
        printf("%d ", poblacion[i].ruta[j]); 
    }
    
    // Al terminar de imprimir TODAS las ciudades de esta ruta, saltamos de línea
    printf("\n"); 
}





free(ciudades); 
    return 0;
}
