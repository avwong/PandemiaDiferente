#include "random.h"
#include <stdio.h>

void crear_random(){
    /*
    S: un numero entero random
    */

    srand(time(NULL)); //set random seed basado en la hora actual
}

int random_intervalo(int min, int max){
    
    /*
    E: intervalo en el que puede estar el numero aleatorio
    S: devuelve un número entero aleatorio entre min y max
    R: min debe ser menor que max
    */

    //Validacion
    if (min >= max) {
        printf("Error: min debe ser menor que max\n");
        return -1; //error
    }

    return rand() % (max - min + 1) + min;
}

//mezcla un arreglo de enteros para luego seleccionar países al azar al mismo tiempo (sin que se repitan)
void mezclar_arreglo(int* arreglo, int totalNumeros){

    /*
    E: un arreglo de enteros y su tamaño
    S: el mismo arreglo pero mezclado aleatoriamente para luego escoger varios de ellos sin que se repitan
    R: que el arreglo exista, el arreglo debe tener al menos 1 elemento
    */

    //validaciones
    if (arreglo == NULL) {
        printf("Error: el arreglo es NULL\n");
        return;
    }

    if (totalNumeros <= 0) {
        printf("Error: el tamaño del arreglo debe ser mayor que 0\n");
        return;
    }

    //mezcla la lista de enteros (algoritmo Fisher-Yates)
    for (int i = totalNumeros - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temp;
    }
}