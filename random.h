#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <time.h>

void crear_random(); //inicializa el generador de números aleatorios
int random_intervalo(int min, int max); //devuelve un número entero aleatorio entre min y max
void mezclar_arreglo(int* arreglo, int totalNumeros); //mezcla un arreglo de enteros para luego seleccionar países al azar al mismo tiempo (sin que se repitan)


#endif

