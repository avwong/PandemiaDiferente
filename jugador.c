#include "jugador.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void crear_jugador(struct jugador *jugador, char* nombre, struct pais* paisInicio){
    /*
    E: puntero al jugador a crear, nombre del jugador, pais en donde inicia el jugador
    S: el jugador creado
    R: que el puntero al jugador no sea NULL, que el país de inicio exista
    */

    //validaciones
    if (jugador == NULL) {
        printf("Error: el puntero al jugador es NULL\n");
        return;
    }

    if (paisInicio == NULL) {
        printf("Error: el país de inicio es NULL\n");
        return;
    }

    //asignar memoria para el nombre
    jugador->nombre = calloc(strlen(nombre) + 1, sizeof(char));
    if (jugador->nombre == NULL) {
        printf("Error: no se pudo asignar memoria para el nombre del jugador\n");
        return;
    }
    strcpy(jugador->nombre, nombre); //copiar el nombre en el la memoria asignada

    jugador->posicion = paisInicio;
    jugador->acciones_restantes = 4; //inicia con 4 acciones

}

// Mueve el jugador de su país actual a otro destino
int mover(struct jugador* jugador, char direccion){

    /*
    E: puntero del jugador a mover, direccion a la que se quiere mover (N, S, E, O)
    S: 0 si el movimiento fue exitoso, -1 si no
    R: que el jugador exista, que la direccion sea valida, que el pais destino exista
    */

    //validaciones
    if (jugador == NULL) {
        printf("Error: el puntero al jugador es NULL\n");
        return -1;
    }

        if (direccion != 'N' && direccion != 'n' && direccion != 'S' && direccion != 's' &&
        direccion != 'E' && direccion != 'e' && direccion != 'O' && direccion != 'o') {
        printf("Direccion invalida. Usa N/S/E/O.\n");
        return -1; //error
    }

    struct pais* destino = NULL; //para apuntar al pais al que se quiere mover

    //la posicion del jugador es un puntero del pais en el que esta actualmente
    if (direccion == 'N' || direccion == 'n') {
        destino = jugador->posicion->norte; //apunta al vecino
    }
    else if (direccion == 'S' || direccion == 's'){ 
        destino = jugador->posicion->sur;
    }
    else if (direccion == 'E' || direccion == 'e'){ 
        destino = jugador->posicion->este;
    }
    else if (direccion == 'O' || direccion == 'o'){
        destino = jugador->posicion->oeste;
    }

    //verificar que en el destino haya un pais
    if (destino == NULL) {
        printf("No hay pais vecino en esa direccion, asi que no te puedes mover en esa direccion.\n");
        return -1; //error
    }

    jugador->posicion = destino; //mueve al jugador al pais destino
    jugador->acciones_restantes--; //se le quita 1 posibilidad de accion ya utilizada
    printf("%s se movió a %s. Acciones restantes: %d\n", jugador->nombre, jugador->posicion->nombre, jugador->acciones_restantes);

    return 0; //se logro realizar el movimiento

}