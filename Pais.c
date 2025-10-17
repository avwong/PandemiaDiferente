#include "Pais.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//crea un país
struct pais* crear_pais(char *nombre) {
    /*
    E: nombre del pais
    S: el país creado
    R: tener memoria para el nuevo pais
    */

    struct pais* nuevoPais = calloc(1, sizeof(struct pais)); //reserva memoria

    //verificar que se haya creado memoria para el nuevo país
    if (nuevoPais == NULL) {
        return NULL; //algo anda mal
    }

    nuevoPais->nombre = nombre;
    nuevoPais->n_aspectos = 0; //inicia sin aspectos
    nuevoPais->n_vecinos = 0; //inicia sin vecinos
    nuevoPais->ant = NULL;
    nuevoPais->sigt = NULL;

    return nuevoPais;
}

//crea la lista de países
struct lista_paises* crear_lista_paises() {
    /*
    E: 
    S: la lista de países creada
    R: que se haya creado memoria
    */

    //verificar que se haya creado memoria para la nueva lista
    struct lista_paises* nuevaLista = calloc(1, sizeof(struct lista_paises));
    if (nuevaLista == NULL) {
        return NULL; //algo anda mal
    }

    nuevaLista->inicio = NULL; //puntero al inicio de la lista
    nuevaLista->fin = NULL; //puntero al final de la lista
    nuevaLista->tamano = 0; //inicia con tamaño 0

    return nuevaLista;
}


int insertar_pais(struct lista_paises* lista, struct pais* nuevoPais){
    /*
    E: la lista de paises, el nuevo pais a insertar_final
    S: no retorna nada, pero el nuevoPais se agrega a lista_paises
    R: que la lista exista, que la lista no este vacia
    */

    //validar que nuev0Pais tenga memoria
    if (nuevoPais == NULL){
        return -1; //error
    }

    //si la lista esta vacia, el nuevo país es el primero
    if (lista->inicio == NULL){
        lista->inicio = nuevoPais;
        lista->fin = nuevoPais; 
        lista->tamano++; //se suma 1 al tamano de la lista
        return 0; //todo en orden

    }else{ //si ya hay países en la lista

        struct pais* actual = lista->inicio; //crear el puntero actual para recorrer la lista

        while (actual != NULL){ //mientras no se haya salido de la lista, siga repitiendo
            if(actual->sigt == NULL){ //si esto pasa es porque aqui tengo que insertar (es el último país)
                actual->sigt = nuevoPais; //pega el nuevo país con el final del actual
                nuevoPais->ant = actual; // pega el país con el anterior
                lista->fin = nuevoPais; //actualiza el puntero al final de la lista
                lista->tamano++;
                return 0; //todo esta bien
            }

            actual = actual->sigt; //avanza al siguiente nodo
        }
    }
    return -1; //termina la funcion y no se inserto nada
}


struct pais* buscar_pais(struct lista_paises* lista, char *nombreABuscar){
    /*
    E: lista de paises, nombre del país a buscar
    S: el país encontrado, NULL si no se encuentra
    R: que la lista exista, que la lista no este vacia
    */

    //validar que la lista exista y no este vacia
    if (lista == NULL || lista->inicio == NULL){
        return NULL; //error
    }

    struct pais* actual = lista->inicio; //crear el puntero actual para recorrer la lista

    while (actual != NULL){ //mientras no se haya salido de la lista, siga repitiendo
        if (strcmp(actual->nombre, nombreABuscar) == 0){ //si el nombre del país actual es igual al nombre buscado
            return actual; //retorna el país encontrado
        }
        actual = actual->sigt; //avanza al siguiente nodo
    }

    return NULL; //no se encontró el país
}


void agregar_vecino(struct pais* paisLocal, struct pais* vecino){
    /*
    E: pais local, pais vecino a agregar
    S: el pais local con el nuevo vecino agregado
    R: que ambos paises existan, que el pais local no tenga más de 5 vecinos
    */

    //validar que ambos países existan
    if (paisLocal == NULL || vecino == NULL){
        printf("Error: uno de los países no existe.\n");
        return;
    }

    //validar que el país local no tenga más de 5 vecinos
    if (paisLocal->n_vecinos >= 4){
        printf("Error: el país %s ya tiene el número máximo de vecinos.\n", paisLocal->nombre);
        return;
    }

    //agregar al vecino a la lista de vecinos del país local
    paisLocal->vecinos[paisLocal->n_vecinos] = vecino;
    paisLocal->n_vecinos++; //aumenta en 1 el número de vecinos

    //viceversa: hacer que el vecino tambien tenga de vecino al local
    vecino->vecinos[vecino->n_vecinos] = paisLocal;
    vecino->n_vecinos++; //aumenta en 1 el número de vecinos
}


void eliminar_pais(struct lista_paises* lista, struct pais* paisEliminar){
    /*
    E: lista de paises, pais a eliminar
    S: lista sin el pais eliminado
    R: que la lista exista, que el pais a eliminar exista en la lista
    */

    //validar que la lista y el pais a eliminar existan
    if (lista == NULL || lista->inicio == NULL) {
        printf("Error: La lista de países no existe.\n");
        return;
    }

    if (paisEliminar == NULL){
        printf("Error: El país a eliminar no existe.\n");
        return;
    }

    //verificar que el país a eliminar esté en la lista
    struct pais* paisEnLista = buscar_pais(lista, paisEliminar->nombre);
    if (paisEnLista == NULL || paisEnLista != paisEliminar) {
        printf("Error: El país a eliminar no está en la lista.\n");
        return;
    }

    //Si el pais a eliminar es el primero
    if (lista->inicio == paisEliminar){
        lista->inicio = paisEliminar->sigt; //el inicio se pasa al segundo pais
        if (lista->inicio != NULL) {
            lista->inicio->ant = NULL; //actualizar puntero del actual primer pais
        } else {
            lista->fin = NULL; //si la lista queda vacia, actualizar fin también
        }
        
    } else { 
        //Si el pais a eliminar está en el medio o final
        if(paisEliminar->ant != NULL){
            paisEliminar->sigt->ant = paisEliminar->ant;
        }
        if(paisEliminar->sigt != NULL){
            paisEliminar->ant->sigt = paisEliminar->sigt;
        } else {
            lista->fin = paisEliminar->ant; //si es el último, actualizar fin
        }
    }

    free(paisEliminar); //liberar la memoria del país eliminado
    lista->tamano--; //disminuir el tamaño de la lista
}


//limpia toda la memoria
void liberar_lista(struct lista_paises* lista){
    /*
    E: lista de paises
    S: toda la memoria liberada
    R: que la lista exista
    */

    //validar que la lista exista
    if (lista == NULL) {
        printf("Error: La lista de países no existe.\n");
        return;
    }

    struct pais* actual = lista->inicio;

    //liberar cada país en la lista
    while (actual != NULL) {
        struct pais* siguiente = actual->sigt; //guardar el siguiente país antes de eliminar actual
        free(actual); //liberar el país actual
        actual = siguiente;
    }

    free(lista); //liberar la memoria de la lista
}