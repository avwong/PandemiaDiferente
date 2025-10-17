#ifndef MUNDO_H
#define MUNDO_H

#include <stdio.h>
#include <stdlib.h>

#include "Pais.h"

//Estructura del mundo
struct mundo {
    struct lista_paises* listaPaises; //lista de paises en el mundo
    char* nombreMundo;
};

//FUNCIONES
void conectar_vecino_direccion(struct pais* paisLocal, struct pais* paisVecino, char direccion);

#endif