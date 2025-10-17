#ifndef MUNDO_H
#define MUNDO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jugador.h"
#include "Pais.h"

#define NUM_PAISES 33 //numero total de paises
#define MAX_ASPECTOS 5 //numero maximo de aspectos por pais

//Estructura del mundo
struct mundo {
    struct lista_paises* listaPaises; //lista de paises en el mundo
    char* nombreMundo;
    int numAspectosMundo; //5
    struct jugador j1, j2; //jugadores
};

//FUNCIONES
struct mundo* crear_mundo(int n_aspectos);
void crear_paises(struct mundo* mundo); //agrega los paises al mundo
void conectar_vecino_direccion(struct pais* paisLocal, struct pais* paisVecino, char direccion);
void asignar_aspectos_iniciales(struct mundo* mundo);
void posicionar_jugadores(struct mundo* mundo);
void mostrar_mapa(struct mundo* mundo);
struct pais* buscar_pais_por_diminutivo(struct mundo* mundo, const char* dim);
int pais_perdido(struct pais* pais); //verifica si un pais esta muerto (todos los aspectos en 3)
void contagiar(struct mundo* mundo);
int condicion_victoria(struct mundo* mundo);  // 1 si hay algun aspecto erradicado en todos los países
int condicion_derrota(struct mundo* mundo);   // 1 si quedan <=3 países activos
void liberar_mundo(struct mundo* mundo); //libera la memoria del mundo

#endif
