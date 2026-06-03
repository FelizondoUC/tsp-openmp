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
    Selección aleatoria simple.
*/
Individuo seleccionar_padre(
    Individuo *poblacion
) {

    int idx =
        rand() % TAM_POBLACION;

    return poblacion[idx];
}

/*
    Genera una nueva población
    usando crossover y mutación.
*/
void nueva_generacion(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades
) {

    for (int i = 0; i < TAM_POBLACION; i++) {

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
            seleccionar_padre(poblacion);

        Individuo padre2 =
            seleccionar_padre(poblacion);

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
    }
}
