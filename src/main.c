#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tsp.h"
#include "genetic.h"

#define FRECUENCIA_IMPRESION 1000
#define MEJORA_MINIMA_IMPRESION 0.01

int main(int argc, char *argv[]) {

    int id_ciudad;
    float coord_x, coord_y;
    srand(time(NULL));

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
    fclose(archivo);

    for (int i = 0; i < num_ciudades; i++) {
    printf("Posición arreglo [%d] -> ID: %d, X: %.2f, Y: %.2f\n", 
            i, 
            ciudades[i].id_ciudad, 
            ciudades[i].coord_x, 
            ciudades[i].coord_y);
}
    
    // Utilizamos TAM_POBLACION definido en genetic.h
    Individuo *poblacion = malloc(TAM_POBLACION * sizeof(Individuo));

    for (int i = 0; i < TAM_POBLACION; i++) {
        // Para cada individuo, reservamos memoria para su ruta de ciudades
        poblacion[i].ruta = malloc(num_ciudades * sizeof(int));
        
        // Le asignamos una combinación aleatoria única
        generar_ruta_aleatoria(poblacion[i].ruta, num_ciudades);
        
        // Evaluamos su distancia total y su fitness
        poblacion[i].distancia = distancia_total(ciudades, poblacion[i].ruta, num_ciudades);
        poblacion[i].fitness = 100000.0 / poblacion[i].distancia; // Inversamente proporcional a la distancia
    }

    printf("\n=== Evaluación de la Población Inicial ===\n");
    for (int i = 0; i < TAM_POBLACION; i++) {
        printf("Individuo %2d - Distancia: %8.2f - Fitness: %.6f\n", 
               i, poblacion[i].distancia, poblacion[i].fitness);
        printf("Ruta: ");
        for (int j = 0; j < num_ciudades; j++) {
            printf("%d ", poblacion[i].ruta[j]); 
        }
        printf("\n\n"); 
    }

    Individuo *nueva_poblacion = malloc(TAM_POBLACION * sizeof(Individuo));
    int generacion = 0;
    double mejor_distancia = poblacion[0].distancia;

    while(1){
        nueva_generacion(
            poblacion,
            nueva_poblacion,
            ciudades,
            num_ciudades,
            TAM_TORNEO
        );

        sustituir_poblacion(poblacion, nueva_poblacion);

        int mejor_indice = 0;
        for (int i = 1; i < TAM_POBLACION; i++) {
            if (poblacion[i].distancia < poblacion[mejor_indice].distancia) {
                mejor_indice = i;
            }
        }

        int hubo_mejora =
            poblacion[mejor_indice].distancia <
            mejor_distancia - MEJORA_MINIMA_IMPRESION;

        if (hubo_mejora) {
            mejor_distancia = poblacion[mejor_indice].distancia;
        }

        if (generacion == 0 ||
            hubo_mejora ||
            generacion % FRECUENCIA_IMPRESION == 0) {

            printf("Generación %d - Mejor distancia: %.2f - Fitness: %.6f\n",
                   generacion,
                   poblacion[mejor_indice].distancia,
                   poblacion[mejor_indice].fitness);
        }

        generacion++;
    }

    for (int i = 0; i < TAM_POBLACION; i++) {
        free(poblacion[i].ruta);
    }
    free(nueva_poblacion);
    free(poblacion);
    free(ciudades); 
    return 0;
}
