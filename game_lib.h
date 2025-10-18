#ifndef GAME_LIB_H
#define GAME_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ASPECTOS 5
#define MAX_VECINOS 4
#define T_DIMINUTIVO 4
#define MAX_DESC 200
#define TABLA_SIZE_INICIAL 7
#define FACTOR_CARGA 0.7

/* ============== ESTRUCTURAS ============== */

/**
 * Estructura para representar un país
 */
typedef struct pais {
    char* nombre;
    char diminutivo[T_DIMINUTIVO];
    int aspectos[MAX_ASPECTOS];
    int n_aspectos;
    int n_vecinos;
    struct pais* vecinos[MAX_VECINOS];
    
    struct pais* norte;
    struct pais* sur;
    struct pais* este;
    struct pais* oeste;
    
    struct pais* sigt;
    struct pais* ant;
} Pais;

/**
 * Estructura para la lista doblemente enlazada de países
 */
typedef struct lista_paises {
    struct pais* inicio;
    struct pais* fin;
    int tamano;
} ListaPaises;

/**
 * Estructura para representar un jugador
 */
typedef struct jugador {
    char* nombre;
    struct pais* posicion;
    int acciones_restantes;
} Jugador;

/**
 * Estructura para representar un proyecto
 */
typedef struct {
    char nombre[MAX_DESC];
    char descripcion[MAX_DESC];
    char bibliografia[MAX_DESC];
    int aspecto_objetivo;
    int efectividad;
} Proyecto;

/**
 * Nodo para la tabla de dispersión
 */
typedef struct NodoHash {
    char clave[MAX_DESC];
    Proyecto proyecto;
    struct NodoHash* siguiente;
} NodoHash;

/**
 * Tabla de dispersión
 */
typedef struct {
    NodoHash** tabla;
    int tamanio;
    int elementos;
} TablaHash;

/* ============== FUNCIONES DE LISTA ============== */

ListaPaises* crear_lista_paises() {
    ListaPaises* lista = (ListaPaises*)malloc(sizeof(ListaPaises));
    lista->inicio = NULL;
    lista->fin = NULL;
    lista->tamano = 0;
    return lista;
}

Pais* crear_pais(const char* nombre, const char* diminutivo) {
    Pais* nuevo = (Pais*)malloc(sizeof(Pais));
    nuevo->nombre = (char*)malloc(strlen(nombre) + 1);
    strcpy(nuevo->nombre, nombre);
    
    strncpy(nuevo->diminutivo, diminutivo, T_DIMINUTIVO - 1);
    nuevo->diminutivo[T_DIMINUTIVO - 1] = '\0';
    
    for (int i = 0; i < MAX_ASPECTOS; i++) {
        nuevo->aspectos[i] = 0;
    }
    nuevo->n_aspectos = 0;
    nuevo->n_vecinos = 0;
    
    for (int i = 0; i < MAX_VECINOS; i++) {
        nuevo->vecinos[i] = NULL;
    }
    
    nuevo->norte = NULL;
    nuevo->sur = NULL;
    nuevo->este = NULL;
    nuevo->oeste = NULL;
    nuevo->sigt = NULL;
    nuevo->ant = NULL;
    
    return nuevo;
}

void agregar_vecino(Pais* pais, Pais* vecino) {
    if (pais->n_vecinos < MAX_VECINOS) {
        pais->vecinos[pais->n_vecinos] = vecino;
        pais->n_vecinos++;
    }
}

void conectar_direccional(Pais* origen, Pais* destino, char direccion) {
    agregar_vecino(origen, destino);
    
    switch(direccion) {
        case 'N': origen->norte = destino; break;
        case 'S': origen->sur = destino; break;
        case 'E': origen->este = destino; break;
        case 'O': origen->oeste = destino; break;
    }
}

void insertar_pais(ListaPaises* lista, Pais* pais) {
    if (lista->inicio == NULL) {
        lista->inicio = pais;
        lista->fin = pais;
    } else {
        lista->fin->sigt = pais;
        pais->ant = lista->fin;
        lista->fin = pais;
    }
    lista->tamano++;
}

Pais* buscar_pais(ListaPaises* lista, const char* nombre) {
    Pais* actual = lista->inicio;
    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre) == 0) {
            return actual;
        }
        actual = actual->sigt;
    }
    return NULL;
}

void eliminar_pais(ListaPaises* lista, const char* nombre) {
    Pais* pais = buscar_pais(lista, nombre);
    if (pais == NULL) return;
    
    if (pais->ant != NULL) {
        pais->ant->sigt = pais->sigt;
    } else {
        lista->inicio = pais->sigt;
    }
    
    if (pais->sigt != NULL) {
        pais->sigt->ant = pais->ant;
    } else {
        lista->fin = pais->ant;
    }
    
    free(pais->nombre);
    free(pais);
    lista->tamano--;
}

/* ============== FUNCIONES HASH ============== */

unsigned int funcion_hash(const char* clave, int tamanio) {
    unsigned int hash = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        hash = (hash * 31 + clave[i]) % tamanio;
    }
    return hash;
}

TablaHash* crear_tabla_hash(int tamanio) {
    TablaHash* tabla = (TablaHash*)malloc(sizeof(TablaHash));
    tabla->tamanio = tamanio;
    tabla->elementos = 0;
    tabla->tabla = (NodoHash**)calloc(tamanio, sizeof(NodoHash*));
    return tabla;
}

void redimensionar_tabla(TablaHash* tabla) {
    int nuevo_tamanio = tabla->tamanio * 2 + 1;
    NodoHash** nueva_tabla = (NodoHash**)calloc(nuevo_tamanio, sizeof(NodoHash*));
    
    for (int i = 0; i < tabla->tamanio; i++) {
        NodoHash* actual = tabla->tabla[i];
        while (actual != NULL) {
            NodoHash* siguiente = actual->siguiente;
            unsigned int nuevo_indice = funcion_hash(actual->clave, nuevo_tamanio);
            actual->siguiente = nueva_tabla[nuevo_indice];
            nueva_tabla[nuevo_indice] = actual;
            actual = siguiente;
        }
    }
    
    free(tabla->tabla);
    tabla->tabla = nueva_tabla;
    tabla->tamanio = nuevo_tamanio;
}

void insertar_proyecto(TablaHash* tabla, const char* clave, Proyecto proyecto) {
    float factor = (float)tabla->elementos / tabla->tamanio;
    if (factor > FACTOR_CARGA) {
        redimensionar_tabla(tabla);
    }
    
    unsigned int indice = funcion_hash(clave, tabla->tamanio);
    NodoHash* nuevo = (NodoHash*)malloc(sizeof(NodoHash));
    strcpy(nuevo->clave, clave);
    nuevo->proyecto = proyecto;
    nuevo->siguiente = tabla->tabla[indice];
    tabla->tabla[indice] = nuevo;
    tabla->elementos++;
}

Proyecto* buscar_proyecto(TablaHash* tabla, const char* clave) {
    unsigned int indice = funcion_hash(clave, tabla->tamanio);
    NodoHash* actual = tabla->tabla[indice];
    
    while (actual != NULL) {
        if (strcmp(actual->clave, clave) == 0) {
            return &(actual->proyecto);
        }
        actual = actual->siguiente;
    }
    return NULL;
}

/* ============== FUNCIONES DE JUEGO ============== */

void crear_territorio(ListaPaises* lista) {
    Pais* mexico = crear_pais("Mexico", "MEX");
    Pais* guatemala = crear_pais("Guatemala", "GUA");
    Pais* honduras = crear_pais("Honduras", "HON");
    Pais* nicaragua = crear_pais("Nicaragua", "NIC");
    Pais* costa_rica = crear_pais("Costa Rica", "CRC");
    Pais* panama = crear_pais("Panama", "PAN");
    Pais* colombia = crear_pais("Colombia", "COL");
    Pais* venezuela = crear_pais("Venezuela", "VEN");
    Pais* brasil = crear_pais("Brasil", "BRA");
    Pais* argentina = crear_pais("Argentina", "ARG");
    
    insertar_pais(lista, mexico);
    insertar_pais(lista, guatemala);
    insertar_pais(lista, honduras);
    insertar_pais(lista, nicaragua);
    insertar_pais(lista, costa_rica);
    insertar_pais(lista, panama);
    insertar_pais(lista, colombia);
    insertar_pais(lista, venezuela);
    insertar_pais(lista, brasil);
    insertar_pais(lista, argentina);
    
    conectar_direccional(mexico, guatemala, 'S');
    conectar_direccional(guatemala, mexico, 'N');
    conectar_direccional(guatemala, honduras, 'S');
    conectar_direccional(honduras, guatemala, 'N');
    conectar_direccional(honduras, nicaragua, 'S');
    conectar_direccional(nicaragua, honduras, 'N');
    conectar_direccional(nicaragua, costa_rica, 'S');
    conectar_direccional(costa_rica, nicaragua, 'N');
    conectar_direccional(costa_rica, panama, 'S');
    conectar_direccional(panama, costa_rica, 'N');
    conectar_direccional(panama, colombia, 'S');
    conectar_direccional(colombia, panama, 'N');
    conectar_direccional(colombia, venezuela, 'E');
    conectar_direccional(venezuela, colombia, 'O');
    conectar_direccional(colombia, brasil, 'S');
    conectar_direccional(brasil, colombia, 'N');
    conectar_direccional(venezuela, brasil, 'S');
    conectar_direccional(brasil, venezuela, 'N');
    conectar_direccional(brasil, argentina, 'S');
    conectar_direccional(argentina, brasil, 'N');
}

void diagnostico_inicial(ListaPaises* lista) {
    int indices[9];
    int usados[10] = {0};
    
    for (int i = 0; i < 9; i++) {
        int indice;
        do {
            indice = rand() % lista->tamano;
        } while (usados[indice]);
        usados[indice] = 1;
        indices[i] = indice;
    }
    
    Pais* actual = lista->inicio;
    int contador = 0;
    
    while (actual != NULL) {
        for (int i = 0; i < 9; i++) {
            if (contador == indices[i]) {
                if (i < 3) {
                    int aspecto1 = rand() % MAX_ASPECTOS;
                    int aspecto2;
                    do {
                        aspecto2 = rand() % MAX_ASPECTOS;
                    } while (aspecto2 == aspecto1);
                    
                    actual->aspectos[aspecto1] = 3;
                    actual->aspectos[aspecto2] = 2;
                    
                } else if (i < 6) {
                    int aspecto1 = rand() % MAX_ASPECTOS;
                    int aspecto2;
                    do {
                        aspecto2 = rand() % MAX_ASPECTOS;
                    } while (aspecto2 == aspecto1);
                    
                    actual->aspectos[aspecto1] = 2;
                    actual->aspectos[aspecto2] = 1;
                    
                } else {
                    int aspectos_usados[MAX_ASPECTOS] = {0};
                    for (int j = 0; j < 3; j++) {
                        int aspecto;
                        do {
                            aspecto = rand() % MAX_ASPECTOS;
                        } while (aspectos_usados[aspecto]);
                        aspectos_usados[aspecto] = 1;
                        actual->aspectos[aspecto] = 1;
                    }
                }
                break;
            }
        }
        contador++;
        actual = actual->sigt;
    }
}

void cargar_proyectos(TablaHash* tabla) {
    Proyecto p1 = {
        "Playas libres de plastico",
        "Prohibir el ingreso y venta de productos que contienen plasticos de un solo uso en las zonas costeras.",
        "Gomez J. G., 2025",
        0, 1
    };
    
    Proyecto p2 = {
        "Ayuda comunitaria",
        "Acciones para apoyar a personas vulnerables, como provision de alimentos, agua y refugio",
        "(The Editors of Encyclopaedia Britannica, 2025)",
        1, 1
    };
    
    Proyecto p3 = {
        "Prohibicion de Tala de arboles",
        "Restringir la deforestacion para proteger bosques, conservar la biodiversidad y prevenir la erosion del suelo.",
        "Murillo, J. A. 2025",
        2, 1
    };
    
    Proyecto p4 = {
        "Sustitucion de combustibles fosiles",
        "Sustituir el uso de combustibles fosiles, como el petroleo, carbon y gas natural por energias renovables, como solar, eolica, hidraulica.",
        "Enlight Mexico 2025",
        3, 2
    };
    
    Proyecto p5 = {
        "Campana por el ahorro de agua",
        "Busca promover el uso responsable del agua mediante mensajes difundidos en carteles, videos, etc",
        "ExpokNews 2022",
        4, 2
    };
    
    insertar_proyecto(tabla, "REFOR001", p1);
    insertar_proyecto(tabla, "ENERG002", p2);
    insertar_proyecto(tabla, "RESID003", p3);
    insertar_proyecto(tabla, "AGRIC004", p4);
    insertar_proyecto(tabla, "TRANS005", p5);
}

void expandir_problematica(ListaPaises* lista, Pais* pais, int aspecto) {
    for (int i = 0; i < pais->n_vecinos; i++) {
        Pais* vecino = pais->vecinos[i];
        if (vecino != NULL && vecino->aspectos[aspecto] < 3) {
            vecino->aspectos[aspecto]++;
        }
    }
}

void incrementar_problematicas(ListaPaises* lista) {
    if (lista->tamano == 0) return;
    
    int paises_afectados = (lista->tamano >= 3) ? 3 : lista->tamano;
    int usados[10] = {0};
    
    for (int i = 0; i < paises_afectados; i++) {
        int indice;
        do {
            indice = rand() % lista->tamano;
        } while (usados[indice]);
        usados[indice] = 1;
        
        Pais* actual = lista->inicio;
        for (int j = 0; j < indice; j++) {
            actual = actual->sigt;
        }
        
        int aspecto = rand() % MAX_ASPECTOS;
        
        if (actual->aspectos[aspecto] < 3) {
            actual->aspectos[aspecto]++;
        } else {
            expandir_problematica(lista, actual, aspecto);
        }
    }
}

int pais_perdido(Pais* pais) {
    for (int i = 0; i < MAX_ASPECTOS; i++) {
        if (pais->aspectos[i] != 3) {
            return 0;
        }
    }
    return 1;
}

void verificar_paises_perdidos(ListaPaises* lista) {
    Pais* actual = lista->inicio;
    while (actual != NULL) {
        Pais* siguiente = actual->sigt;
        if (pais_perdido(actual)) {
            printf("\n¡ALERTA! %s ha sido eliminado (todos sus aspectos en nivel 3)\n", actual->nombre);
            eliminar_pais(lista, actual->nombre);
        }
        actual = siguiente;
    }
}

int verificar_victoria(ListaPaises* lista) {
    for (int aspecto = 0; aspecto < MAX_ASPECTOS; aspecto++) {
        int erradicado = 1;
        Pais* actual = lista->inicio;
        
        while (actual != NULL) {
            if (actual->aspectos[aspecto] != 0) {
                erradicado = 0;
                break;
            }
            actual = actual->sigt;
        }
        
        if (erradicado) {
            return 1;
        }
    }
    return 0;
}

int verificar_derrota(ListaPaises* lista) {
    return (lista->tamano <= 3);
}

#endif
