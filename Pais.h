#ifndef PAIS_H
#define PAIS_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_ASPECTOS 5 //constante: número máximo de aspectos que puede tener un pais
#define MAX_VECINOS 4  //constante: número máximo de vecinos que puede tener un pais


//Estructura para representar un país
struct pais{
    //datos del país
    char* nombre;
    int aspectos[MAX_ASPECTOS]; //lista con los aspectos de cada pais
    int n_aspectos; //número actual de aspectos
    int n_vecinos; //número de países vecinos
    struct pais* vecinos[MAX_VECINOS]; //lista de índices de países vecinos
    
    //Vecinos por direccion para poder mostrarlos correctamente en el mapa
    struct pais* norte;
    struct pais* sur;
    struct pais* este;
    struct pais* oeste;

    //punteros
    struct pais* sigt;
    struct pais* ant;

};

//Estructura para representar la lista de países
struct lista_paises{
    struct pais* inicio;  //apunta al primer país
    struct pais* fin;  //apunta al último país
    int tamano;     // cantidad de países
};

//FUNCIONES
struct pais* crear_pais(char* nombre);
struct lista_paises* crear_lista_paises();
int insertar_pais(struct lista_paises* lista, struct pais* nuevoPais);
struct pais* buscar_pais(struct lista_paises* lista, char *nombre);
void agregar_vecino(struct pais* paisLocal, struct pais* vecino);
void eliminar_pais(struct lista_paises* lista, struct pais* paisEliminar);
void liberar_lista(struct lista_paises* lista); //limpia toda la memoria



#endif
