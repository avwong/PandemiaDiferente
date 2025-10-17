#include "Mundo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//paisVecino se coloca en la direccion de paisLocal
void conectar_vecino_direccion(struct pais* paisLocal, struct pais* paisVecino, char direccion){
    /*
    E: el pais local, el pais vecino y la direccion en la cual se conecta (N, S, E, O)
    S: pais local con el vecino conectado al puntero de la direccion indicada
    R: que ambos paises existan, que la direccion de entrada sea valida
    */

    //Validar si los paises existen
    if (paisLocal == NULL || paisVecino == NULL) {
        printf("Error: uno de los países es NULL\n");
        return;
    }
    
    //Validar direccion
    if (direccion != 'N' && direccion != 'S' && direccion != 'E' && direccion != 'O') {
        printf("Error: direccion '%c' invalida. Use N, S, E o O\n", direccion);
        return;
    }

    //Conectar los paises
    if (direccion == 'N') {
        if (paisLocal->norte != NULL) { //si ya tiene vecino en esa direccion
            printf("Advertencia: %s ya tiene un vecino al Norte\n", paisLocal->nombre);
        }
        paisLocal->norte = paisVecino;
        paisVecino->sur = paisLocal; //viceversa
    }

    else if (direccion == 'S') {
        if (paisLocal->sur != NULL) {
            printf("Advertencia: %s ya tiene un vecino al Sur\n", paisLocal->nombre);
        }
        paisLocal->sur = paisVecino;
        paisVecino->norte = paisLocal;
    }

    else if (direccion == 'E') {
        if (paisLocal->este != NULL) {
            printf("Advertencia: %s ya tiene un vecino al Este\n", paisLocal->nombre);
        }
        paisLocal->este = paisVecino;
        paisVecino->oeste = paisLocal;
    }
    
    else if (direccion == 'O') {
        if (paisLocal->oeste != NULL) {
            printf("Advertencia: %s ya tiene un vecino al Oeste\n", paisLocal->nombre);
        }
        paisLocal->oeste = paisVecino;
        paisVecino->este = paisLocal;
    }

}