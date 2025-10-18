#include "game_lib.h"

void mostrar_menu() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                       CAMBIO CLIMATICO                   ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("1. Jugar\n");
    printf("2. Salir\n");
    printf("\nSeleccione una opcion: ");
}

void mostrar_tablero(ListaPaises* lista, Jugador* j1, Jugador* j2) {
    printf("\n╔═════════════════════════ ESTADO DEL MAPA ═════════════════════════════╗\n");
    Pais* actual = lista->inicio;
    
    const char* nombres_aspectos[] = {
        "Contaminacion Agua",
        "Desastres Naturales",
        "Deforestacion",
        "Gases Efecto Invernadero",
        "Explotacion Agua"
    };
    
    while (actual != NULL) {
        char marcador[20] = "";
        if (j1->posicion == actual && j2->posicion == actual) {
            strcpy(marcador, " [J1 y J2]");
        } else if (j1->posicion == actual) {
            strcpy(marcador, " [J1]");
        } else if (j2->posicion == actual) {
            strcpy(marcador, " [J2]");
        }
        
        printf("\n%s (%s)%s:\n", actual->nombre, actual->diminutivo, marcador);
        for (int i = 0; i < MAX_ASPECTOS; i++) {
            printf("  %s: %d  ", nombres_aspectos[i], actual->aspectos[i]);
        }
        printf("\n");
        
        actual = actual->sigt;
    }
    printf("╚══════════════════════════════════════════════════════════════════════╝\n");
}

void mostrar_proyectos(TablaHash* tabla) {
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║              PROYECTOS DISPONIBLES                       ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    
    const char* claves[] = {"REFOR001", "ENERG002", "RESID003", "AGRIC004", "TRANS005"};
    const char* aspectos[] = {
        "Contaminacion Agua", 
        "Desastres Naturales", 
        "Deforestacion",
        "Gases Efecto Invernadero",  
        "Explotacion Agua"
    };
    
    for (int i = 0; i < 5; i++) {
        Proyecto* p = buscar_proyecto(tabla, claves[i]);
        if (p != NULL) {
            printf("%d. %s [%s]\n", i+1, claves[i], p->nombre);
            printf("   Descripcion: %s\n", p->descripcion);
            printf("   Reduce: %s (-%d punto)\n", aspectos[p->aspecto_objetivo], p->efectividad);
            printf("   Fuente: %s\n", p->bibliografia);
        }
    }
}

void inicializar_jugadores(Jugador* j1, Jugador* j2, ListaPaises* lista) {
    j1->nombre = (char*)malloc(20);
    j2->nombre = (char*)malloc(20);
    strcpy(j1->nombre, "JugadorUno");
    strcpy(j2->nombre, "JugadorDos");
    
    j1->acciones_restantes = 4;
    j2->acciones_restantes = 4;
    
    int idx1 = rand() % lista->tamano;
    int idx2;
    do {
        idx2 = rand() % lista->tamano;
    } while (idx1 == idx2);
    
    Pais* actual = lista->inicio;
    for (int i = 0; i < idx1; i++) {
        actual = actual->sigt;
    }
    j1->posicion = actual;
    
    actual = lista->inicio;
    for (int i = 0; i < idx2; i++) {
        actual = actual->sigt;
    }
    j2->posicion = actual;
    
    printf("\n%s comienza en: %s\n", j1->nombre, j1->posicion->nombre);
    printf("%s comienza en: %s\n", j2->nombre, j2->posicion->nombre);
}

int mover_jugador(Jugador* jugador, Pais* destino) {
    Pais* pais_actual = jugador->posicion;
    if (pais_actual == NULL) return 0;
    
    for (int i = 0; i < pais_actual->n_vecinos; i++) {
        if (pais_actual->vecinos[i] == destino) {
            jugador->posicion = destino;
            printf("%s se ha movido a %s\n", jugador->nombre, destino->nombre);
            return 1;
        }
    }
    
    printf("Error: %s no es vecino de %s\n", destino->nombre, pais_actual->nombre);
    return 0;
}

int implementar_proyecto(Jugador* jugador, Proyecto* proyecto) {
    Pais* pais = jugador->posicion;
    if (pais == NULL) return 0;
    
    int aspecto = proyecto->aspecto_objetivo;
    if (pais->aspectos[aspecto] > 0) {
        pais->aspectos[aspecto] -= proyecto->efectividad;
        if (pais->aspectos[aspecto] < 0) {
            pais->aspectos[aspecto] = 0;
        }
        printf("Proyecto '%s' implementado en %s\n", proyecto->nombre, pais->nombre);
        printf("Aspecto reducido a: %d\n", pais->aspectos[aspecto]);
        return 1;
    } else {
        printf("Este aspecto ya esta en 0 en %s\n", pais->nombre);
        return 0;
    }
}

void turno_jugador(Jugador* jugador, TablaHash* tabla) {
    jugador->acciones_restantes = 4;
    
    printf("\n========== TURNO DE %s ==========\n", jugador->nombre);
    printf("Ubicacion actual: %s (%s)\n", jugador->posicion->nombre, jugador->posicion->diminutivo);
    
    Pais* pais_actual = jugador->posicion;
    
    while (jugador->acciones_restantes > 0) {
        printf("\n--- Acciones restantes: %d ---\n", jugador->acciones_restantes);
        printf("1. Moverse a pais vecino\n");
        printf("2. Implementar proyecto\n");
        printf("3. Ver estado del pais actual\n");
        printf("4. Terminar turno\n");
        printf("Opcion: ");
        
        int opcion;
        scanf("%d", &opcion);
        getchar();
        
        switch(opcion) {
            case 1: {
                printf("Paises vecinos:\n");
                for (int i = 0; i < pais_actual->n_vecinos; i++) {
                    char direccion[10] = "";
                    if (pais_actual->vecinos[i] == pais_actual->norte) strcpy(direccion, "(Norte)");
                    else if (pais_actual->vecinos[i] == pais_actual->sur) strcpy(direccion, "(Sur)");
                    else if (pais_actual->vecinos[i] == pais_actual->este) strcpy(direccion, "(Este)");
                    else if (pais_actual->vecinos[i] == pais_actual->oeste) strcpy(direccion, "(Oeste)");
                    
                    printf("  %d. %s %s\n", i+1, pais_actual->vecinos[i]->nombre, direccion);
                }
                printf("Seleccione destino (1-%d): ", pais_actual->n_vecinos);
                
                int dest;
                scanf("%d", &dest);
                getchar();
                
                if (dest > 0 && dest <= pais_actual->n_vecinos) {
                    if (mover_jugador(jugador, pais_actual->vecinos[dest-1])) {
                        jugador->acciones_restantes--;
                        pais_actual = jugador->posicion;
                    }
                }
                break;
            }
            
            case 2: {
                mostrar_proyectos(tabla);
                printf("Seleccione proyecto (1-5): ");
                
                int proj;
                scanf("%d", &proj);
                getchar();
                
                const char* claves[] = {"REFOR001", "ENERG002", "RESID003", "AGRIC004", "TRANS005"};
                if (proj > 0 && proj <= 5) {
                    Proyecto* p = buscar_proyecto(tabla, claves[proj-1]);
                    if (p != NULL && implementar_proyecto(jugador, p)) {
                        jugador->acciones_restantes--;
                    }
                }
                break;
            }
            
            case 3: {
                const char* nombres_aspectos[] = {
                    "Contaminacion Agua",
                    "Desastres Naturales",
                    "Deforestacion",
                    "Gases Efecto Invernadero",
                    "Explotacion Agua"
                };
                
                printf("\nEstado de %s:\n", pais_actual->nombre);
                for (int i = 0; i < MAX_ASPECTOS; i++) {
                    printf("  %s: %d\n", nombres_aspectos[i], pais_actual->aspectos[i]);
                }
                break;
            }
            
            case 4:
                jugador->acciones_restantes = 0;
                break;
                
            default:
                printf("Opcion invalida\n");
        }
    }
}

void jugar() {
    ListaPaises* lista = crear_lista_paises();
    TablaHash* tabla = crear_tabla_hash(TABLA_SIZE_INICIAL);
    Jugador j1, j2;
    int turno = 0;
    
    printf("\n╔══════════════════════════════════════════════════════════╗\n");
    printf("║              INICIANDO JUEGO...                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    
    printf("\nSe esta creando territorio de America Latina, con su diagnostico\n");
    crear_territorio(lista);
    cargar_proyectos(tabla);
    diagnostico_inicial(lista);
    inicializar_jugadores(&j1, &j2, lista);
    
    printf("\nJuego iniciado :)\n");
    printf("Presione Enter para comenzar...");
    getchar();
    
    while (1) {
        turno++;
        printf("\n\n");
        printf("╔══════════════════════════════════════════════════════════╗\n");
        printf("║                    TURNO %d                              ║\n", turno);
        printf("╚══════════════════════════════════════════════════════════╝\n");
        
        mostrar_tablero(lista, &j1, &j2);
        
        turno_jugador(&j1, tabla);
        
        if (verificar_victoria(lista)) {
            printf("\n\n");
            printf("╔══════════════════════════════════════════════════════════╗\n");
            printf("║                  ¡ Ganaste :) !                          ║\n");
            printf("╚══════════════════════════════════════════════════════════╝\n");
            printf("\n¡Han erradicado un aspecto del cambio climatico en todos los paises!\n");
            printf("Turnos jugados: %d\n", turno);
            break;
        }
        
        turno_jugador(&j2, tabla);
        
        if (verificar_victoria(lista)) {
            printf("\n\n");
            printf("╔══════════════════════════════════════════════════════════╗\n");
            printf("║                  ¡ Ganaste :) !                          ║\n");
            printf("╚══════════════════════════════════════════════════════════╝\n");
            printf("\n¡Han erradicado un aspecto del cambio climatico en todos los paises!\n");
            printf("Turnos jugados: %d\n", turno);
            break;
        }
        
        printf("\n--- FASE DE EXPANSION DE PROBLEMATICAS ---\n");
        incrementar_problematicas(lista);
        
        verificar_paises_perdidos(lista);
        
        if (verificar_derrota(lista)) {
            printf("\n\n");
            printf("╔══════════════════════════════════════════════════════════╗\n");
            printf("║                  Has Perdido                             ║\n");
            printf("╚══════════════════════════════════════════════════════════╝\n");
            printf("\nLa crisis se ha salido de control.\n");
            printf("Solo quedan %d paises en el tablero:(.\n", lista->tamano);
            printf("Turnos jugados: %d\n", turno);
            break;
        }
        
        printf("\nPresione Enter para continuar al siguiente turno...");
        getchar();
    }
    
    // Liberar memoria
    Pais* actual = lista->inicio;
    while (actual != NULL) {
        Pais* siguiente = actual->sigt;
        free(actual->nombre);
        free(actual);
        actual = siguiente;
    }
    free(lista);
    
    free(j1.nombre);
    free(j2.nombre);
    
    for (int i = 0; i < tabla->tamanio; i++) {
        NodoHash* nodo = tabla->tabla[i];
        while (nodo != NULL) {
            NodoHash* siguiente = nodo->siguiente;
            free(nodo);
            nodo = siguiente;
        }
    }
    free(tabla->tabla);
    free(tabla);
}

int main() {
    srand(time(NULL));
    int opcion;
    
    while (1) {
        mostrar_menu();
        scanf("%d", &opcion);
        getchar();
        
        switch(opcion) {
            case 1:
                jugar();
                break;
                
            case 2:
                printf("\nGracias por jugar :)\n");
                return 0;
                
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
        }
    }
    
    return 0;
}
