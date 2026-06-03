/*
    Realiza mutación
    intercambiando ciudades.
*/
void mutacion(
    Individuo *individuo,
    int num_ciudades
);

/*
    Realiza crossover
    entre dos padres.
*/
void crossover(
    Individuo *padre1,
    Individuo *padre2,
    Individuo *hijo,
    int num_ciudades
);

/*
    Selecciona un individuo aleatorio.
*/
Individuo seleccionar_padre(
    Individuo *poblacion
);

/*
    Genera una nueva población.
*/
void nueva_generacion(
    Individuo *poblacion,
    Individuo *nueva_poblacion,
    Ciudad *ciudades,
    int num_ciudades
);
