#ifndef JUGADOR_H
#define JUGADOR_H

#include "Pais.h"

// Estructura para representar un jugador
struct jugador{
    char* nombre;
    struct pais* posicion; //país donde está el jugador actualmente
};

//FUNCIONES
void crear_jugador(struct jugador *jugador, char* nombre, struct pais* inicio);

// Mueve el jugador de su país actual a un vecino
int mover(struct jugador* jugador, char direccion);

#endif

