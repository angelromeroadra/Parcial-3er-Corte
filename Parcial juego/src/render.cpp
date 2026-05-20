#include "../include/render.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// -- dale pues... limpio la pantalla de forma portable --
void limpiar_pantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// -- a ver... dibujo la barra de salud con ASCII (innovacion 2) --
void dibujar_barra_salud(int vida_actual, int vida_maxima) {
    int segmentos_totales = 10;
    int segmentos_llenos;
    int indice;

    if (vida_maxima <= 0) {
        vida_maxima = 1;
    }
    segmentos_llenos = (vida_actual * segmentos_totales) / vida_maxima;

    printf("HP [");
    for (indice = 0; indice < segmentos_totales; indice++) {
        if (indice < segmentos_llenos) {
            printf("#");
        } else {
            printf("-");
        }
    }
    printf("] %d/%d\n", vida_actual, vida_maxima);
}

// -- mira que... dibujo el mapa del mundo con niebla de guerra (innovacion 1) --
void dibujar_mapa_mundo(EstadoJuego *estado) {
    // Disposicion del mapa de 6 habitaciones en grilla 2x3:
    // [0][1]
    // [2][3]
    // [4][5]
    int grilla_filas[6]     = {0, 0, 1, 1, 2, 2};
    int grilla_columnas[6]  = {0, 1, 0, 1, 0, 1};
    int tablero[3][2];
    int fila;
    int columna;
    int identificador;
    int jugador_fila;
    int jugador_columna;

    // inicializar tablero
    for (fila = 0; fila < 3; fila++) {
        for (columna = 0; columna < 2; columna++) {
            tablero[fila][columna] = -1;
        }
    }

    for (identificador = 0; identificador < MAXIMO_HABITACIONES; identificador++) {
        tablero[grilla_filas[identificador]][grilla_columnas[identificador]] = identificador;
    }

    jugador_fila    = grilla_filas[estado->jugador.habitacion_actual];
    jugador_columna = grilla_columnas[estado->jugador.habitacion_actual];

    printf("-- Mapa del Dungeon --\n");
    for (fila = 0; fila < 3; fila++) {
        for (columna = 0; columna < 2; columna++) {
            identificador = tablero[fila][columna];
            if (identificador == -1) {
                printf("[   ]");
            } else if (fila == jugador_fila && columna == jugador_columna) {
                printf("[@%2d]", identificador);
            } else if (estado->lista_habitaciones[identificador].visitada ||
                       estado->mapa_revelado) {
                printf("[ %2d]", identificador);
            } else {
                printf("[ ? ]");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// -- bueno pues... dibujo la habitacion con todos sus habitantes --
void dibujar_habitacion(EstadoJuego *estado) {
    // copiar la cuadricula base de la habitacion actual
    char cuadricula_temporal[ALTO_HABITACION][ANCHO_HABITACION];
    int habitacion_id = estado->jugador.habitacion_actual;
    int fila;
    int columna;
    int indice;

    for (fila = 0; fila < ALTO_HABITACION; fila++) {
        for (columna = 0; columna < ANCHO_HABITACION; columna++) {
            cuadricula_temporal[fila][columna] =
                estado->lista_habitaciones[habitacion_id].cuadricula[fila][columna];
        }
    }

    // colocar objetos en la cuadricula temporal
    for (indice = 0; indice < estado->cantidad_objetos; indice++) {
        Objeto *objeto = &estado->lista_objetos[indice];
        if (!objeto->recogido && objeto->habitacion_actual == habitacion_id) {
            int of = objeto->posicion.fila;
            int oc = objeto->posicion.columna;
            if (of >= 0 && of < ALTO_HABITACION && oc >= 0 && oc < ANCHO_HABITACION) {
                cuadricula_temporal[of][oc] = CELDA_OBJETO;
            }
        }
    }

    // colocar enemigos en la cuadricula temporal
    for (indice = 0; indice < estado->cantidad_enemigos; indice++) {
        Enemigo *enemigo = &estado->lista_enemigos[indice];
        if (enemigo->esta_vivo && enemigo->habitacion_actual == habitacion_id) {
            int ef = enemigo->posicion.fila;
            int ec = enemigo->posicion.columna;
            if (ef >= 0 && ef < ALTO_HABITACION && ec >= 0 && ec < ANCHO_HABITACION) {
                if (enemigo->tipo == TIPO_GUARDIA) {
                    cuadricula_temporal[ef][ec] = CELDA_GUARDIA;
                } else {
                    cuadricula_temporal[ef][ec] = CELDA_FANTASMA;
                }
            }
        }
    }

    // colocar jugador
    int jf = estado->jugador.posicion.fila;
    int jc = estado->jugador.posicion.columna;
    if (jf >= 0 && jf < ALTO_HABITACION && jc >= 0 && jc < ANCHO_HABITACION) {
        cuadricula_temporal[jf][jc] = CELDA_JUGADOR;
    }

    // dibujar cuadricula
    for (fila = 0; fila < ALTO_HABITACION; fila++) {
        for (columna = 0; columna < ANCHO_HABITACION; columna++) {
            printf("%c ", cuadricula_temporal[fila][columna]);
        }
        printf("\n");
    }
}

// -- oye mira... junto todo el estado en pantalla --
void dibujar_estado(EstadoJuego *estado) {
    int habitacion_id = estado->jugador.habitacion_actual;
    Jugador *jugador  = &estado->jugador;

    limpiar_pantalla();

    printf("=========================================\n");
    printf("   DUNGEON CRAWLER - Parcial Final\n");
    printf("=========================================\n\n");

    dibujar_mapa_mundo(estado);

    printf("Habitacion %d: %s\n\n",
           habitacion_id,
           estado->lista_habitaciones[habitacion_id].descripcion);

    dibujar_habitacion(estado);

    printf("\n");
    dibujar_barra_salud(jugador->puntos_de_vida, jugador->puntos_de_vida_maximo);

    if (jugador->tiene_objeto) {
        printf("Inventario: [Llave]\n");
    } else {
        printf("Inventario: [vacio]\n");
    }

    printf("\nControles: W/A/S/D=Mover  E=Recoger/Soltar  Q=Salir\n");
    printf("Leyenda: @=Tu  G=Guardia  F=Fantasma  *=Objeto  E=Salida\n");
}

// -- listo ya... muestro el resumen final de la partida (innovacion 7) --
void mostrar_estadisticas_finales(EstadoJuego *estado) {
    int habitaciones_visitadas = 0;
    int indice;

    for (indice = 0; indice < MAXIMO_HABITACIONES; indice++) {
        if (estado->lista_habitaciones[indice].visitada) {
            habitaciones_visitadas++;
        }
    }

    printf("\n=========================================\n");
    if (estado->estado_partida == ESTADO_VICTORIA) {
        printf("   *** VICTORIA! Escapaste del dungeon ***\n");
    } else {
        printf("   *** DERROTA. El dungeon te venció ***\n");
    }
    printf("=========================================\n");
    printf("Pasos dados          : %d\n", estado->jugador.pasos_dados);
    printf("Enemigos eliminados  : %d\n", estado->jugador.enemigos_eliminados);
    printf("Habitaciones visitadas: %d / %d\n", habitaciones_visitadas, MAXIMO_HABITACIONES);
    printf("=========================================\n\n");
}
