#include "../include/enemigos.h"

// -- a ver... el guardia persigue en una sola dimension (innovacion 4) --
void mover_guardia(EstadoJuego *estado, Enemigo *enemigo) {
    Jugador *jugador    = &estado->jugador;
    Habitacion *habitacion = &estado->lista_habitaciones[enemigo->habitacion_actual];
    int diferencia_fila    = jugador->posicion.fila    - enemigo->posicion.fila;
    int diferencia_columna = jugador->posicion.columna - enemigo->posicion.columna;
    int nueva_fila    = enemigo->posicion.fila;
    int nueva_columna = enemigo->posicion.columna;

    // el guardia prioriza el eje con mayor diferencia
    if (diferencia_fila < 0) diferencia_fila = -diferencia_fila;
    if (diferencia_columna < 0) diferencia_columna = -diferencia_columna;

    if (diferencia_fila >= diferencia_columna) {
        if (jugador->posicion.fila > enemigo->posicion.fila) {
            nueva_fila++;
        } else if (jugador->posicion.fila < enemigo->posicion.fila) {
            nueva_fila--;
        }
    } else {
        if (jugador->posicion.columna > enemigo->posicion.columna) {
            nueva_columna++;
        } else if (jugador->posicion.columna < enemigo->posicion.columna) {
            nueva_columna--;
        }
    }

    // el guardia respeta las paredes
    if (nueva_fila >= 0 && nueva_fila < ALTO_HABITACION &&
        nueva_columna >= 0 && nueva_columna < ANCHO_HABITACION &&
        habitacion->cuadricula[nueva_fila][nueva_columna] != CELDA_PARED) {
        enemigo->posicion.fila    = nueva_fila;
        enemigo->posicion.columna = nueva_columna;
    }
}

// -- dale que va... el fantasma atraviesa paredes internas (innovacion 4) --
void mover_fantasma(EstadoJuego *estado, Enemigo *enemigo) {
    Jugador *jugador = &estado->jugador;
    int nueva_fila    = enemigo->posicion.fila;
    int nueva_columna = enemigo->posicion.columna;

    // el fantasma avanza diagonalmente hacia el jugador sin importar paredes internas
    if (jugador->posicion.fila > enemigo->posicion.fila) {
        nueva_fila++;
    } else if (jugador->posicion.fila < enemigo->posicion.fila) {
        nueva_fila--;
    }

    if (jugador->posicion.columna > enemigo->posicion.columna) {
        nueva_columna++;
    } else if (jugador->posicion.columna < enemigo->posicion.columna) {
        nueva_columna--;
    }

    // el fantasma solo respeta los bordes exteriores
    if (nueva_fila > 0 && nueva_fila < ALTO_HABITACION - 1 &&
        nueva_columna > 0 && nueva_columna < ANCHO_HABITACION - 1) {
        enemigo->posicion.fila    = nueva_fila;
        enemigo->posicion.columna = nueva_columna;
    }
}

// -- oye mira... resuelvo el combate cuando se superponen posiciones --
void resolver_combate(EstadoJuego *estado) {
    Jugador *jugador = &estado->jugador;
    int indice;

    for (indice = 0; indice < estado->cantidad_enemigos; indice++) {
        Enemigo *enemigo = &estado->lista_enemigos[indice];
        if (!enemigo->esta_vivo) {
            continue;
        }
        if (enemigo->habitacion_actual != jugador->habitacion_actual) {
            continue;
        }
        if (enemigo->posicion.fila    == jugador->posicion.fila &&
            enemigo->posicion.columna == jugador->posicion.columna) {
            // el jugador golpea al enemigo
            enemigo->puntos_de_vida--;

            if (enemigo->puntos_de_vida <= 0) {
                enemigo->esta_vivo = 0;
                jugador->enemigos_eliminados++;
            } else {
                // el enemigo contraataca
                jugador->puntos_de_vida -= 2;
            }
        }
    }

    if (jugador->puntos_de_vida <= 0) {
        estado->estado_partida = ESTADO_DERROTA;
    }
}

// -- bueno entonces... actualizo todos los enemigos en la habitacion actual --
void actualizar_enemigos(EstadoJuego *estado) {
    int indice;

    for (indice = 0; indice < estado->cantidad_enemigos; indice++) {
        Enemigo *enemigo = &estado->lista_enemigos[indice];
        if (!enemigo->esta_vivo) {
            continue;
        }
        if (enemigo->habitacion_actual != estado->jugador.habitacion_actual) {
            continue;
        }

        if (enemigo->tipo == TIPO_GUARDIA) {
            mover_guardia(estado, enemigo);
        } else {
            mover_fantasma(estado, enemigo);
        }
    }

    resolver_combate(estado);
}
