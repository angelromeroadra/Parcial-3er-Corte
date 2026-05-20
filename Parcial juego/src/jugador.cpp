#include "../include/jugador.h"
#include "../include/render.h"
#include <stdio.h>

// -- bueno entonces... calculo si una posicion es transitable --
static int posicion_libre(Habitacion *habitacion, int fila, int columna) {
    if (fila < 0 || fila >= ALTO_HABITACION ||
        columna < 0 || columna >= ANCHO_HABITACION) {
        return 0;
    }
    char celda = habitacion->cuadricula[fila][columna];
    return (celda != CELDA_PARED);
}

// -- a ver... reviso si hay cambio de habitacion por las puertas --
static void revisar_cambio_habitacion(EstadoJuego *estado) {
    Jugador *jugador      = &estado->jugador;
    int habitacion_actual = jugador->habitacion_actual;
    Habitacion *habitacion = &estado->lista_habitaciones[habitacion_actual];
    int nueva_habitacion  = -1;
    int nueva_fila        = jugador->posicion.fila;
    int nueva_columna     = jugador->posicion.columna;
    int puerta_fila;
    int puerta_columna;

    // verificar puerta norte
    puerta_fila    = 0;
    puerta_columna = ANCHO_HABITACION / 2;
    if (jugador->posicion.fila == puerta_fila &&
        jugador->posicion.columna == puerta_columna &&
        habitacion->norte != -1) {
        nueva_habitacion = habitacion->norte;
        nueva_fila       = ALTO_HABITACION - 2;
        nueva_columna    = ANCHO_HABITACION / 2;
    }

    // verificar puerta sur
    puerta_fila    = ALTO_HABITACION - 1;
    puerta_columna = ANCHO_HABITACION / 2;
    if (jugador->posicion.fila == puerta_fila &&
        jugador->posicion.columna == puerta_columna &&
        habitacion->sur != -1) {
        nueva_habitacion = habitacion->sur;
        nueva_fila       = 1;
        nueva_columna    = ANCHO_HABITACION / 2;
    }

    // verificar puerta este
    puerta_fila    = ALTO_HABITACION / 2;
    puerta_columna = ANCHO_HABITACION - 1;
    if (jugador->posicion.fila == puerta_fila &&
        jugador->posicion.columna == puerta_columna &&
        habitacion->este != -1) {
        nueva_habitacion = habitacion->este;
        nueva_fila       = ALTO_HABITACION / 2;
        nueva_columna    = 1;
    }

    // verificar puerta oeste
    puerta_fila    = ALTO_HABITACION / 2;
    puerta_columna = 0;
    if (jugador->posicion.fila == puerta_fila &&
        jugador->posicion.columna == puerta_columna &&
        habitacion->oeste != -1) {
        nueva_habitacion = habitacion->oeste;
        nueva_fila       = ALTO_HABITACION / 2;
        nueva_columna    = ANCHO_HABITACION - 2;
    }

    if (nueva_habitacion != -1) {
        jugador->habitacion_actual    = nueva_habitacion;
        jugador->posicion.fila        = nueva_fila;
        jugador->posicion.columna     = nueva_columna;
        estado->lista_habitaciones[nueva_habitacion].visitada = 1;
    }
}

// -- dale pues... muevo al jugador segun la tecla --
void mover_jugador(EstadoJuego *estado, char direccion) {
    Jugador *jugador      = &estado->jugador;
    Habitacion *habitacion = &estado->lista_habitaciones[jugador->habitacion_actual];
    int nueva_fila    = jugador->posicion.fila;
    int nueva_columna = jugador->posicion.columna;

    if (direccion == 'w' || direccion == 'W') {
        nueva_fila--;
    } else if (direccion == 's' || direccion == 'S') {
        nueva_fila++;
    } else if (direccion == 'a' || direccion == 'A') {
        nueva_columna--;
    } else if (direccion == 'd' || direccion == 'D') {
        nueva_columna++;
    }

    if (posicion_libre(habitacion, nueva_fila, nueva_columna)) {
        jugador->posicion.fila    = nueva_fila;
        jugador->posicion.columna = nueva_columna;
        jugador->pasos_dados++;
        revisar_cambio_habitacion(estado);
    }
}

// -- oye mira... el jugador recoge o suelta el objeto en la misma posicion --
void recoger_o_soltar_objeto(EstadoJuego *estado) {
    Jugador *jugador = &estado->jugador;
    int indice;

    if (jugador->tiene_objeto == 0) {
        // intentar recoger
        for (indice = 0; indice < estado->cantidad_objetos; indice++) {
            Objeto *objeto = &estado->lista_objetos[indice];
            if (!objeto->recogido &&
                objeto->habitacion_actual == jugador->habitacion_actual &&
                objeto->posicion.fila == jugador->posicion.fila &&
                objeto->posicion.columna == jugador->posicion.columna) {
                objeto->recogido      = 1;
                jugador->tiene_objeto = 1;

                // innovacion 5: la llave revela el mapa completo
                if (objeto->es_llave) {
                    estado->mapa_revelado = 1;
                    printf("\n¡Recogiste la Llave! El mapa se revela...\n");
                }
                return;
            }
        }
        printf("\nNo hay nada aqui para recoger.\n");
    } else {
        // soltar objeto en la posicion actual
        for (indice = 0; indice < estado->cantidad_objetos; indice++) {
            Objeto *objeto = &estado->lista_objetos[indice];
            if (objeto->recogido) {
                objeto->recogido              = 0;
                objeto->habitacion_actual     = jugador->habitacion_actual;
                objeto->posicion.fila         = jugador->posicion.fila;
                objeto->posicion.columna      = jugador->posicion.columna;
                jugador->tiene_objeto         = 0;

                // si la llave se suelta, el mapa vuelve a oscurecerse
                if (objeto->es_llave) {
                    estado->mapa_revelado = 0;
                }
                printf("\nSoltaste: %s\n", objeto->nombre);
                return;
            }
        }
    }
}

// -- bueno pues... reviso si el jugador esta en la salida con la llave --
void revisar_salida(EstadoJuego *estado) {
    Jugador *jugador      = &estado->jugador;
    int habitacion_actual = jugador->habitacion_actual;
    Habitacion *habitacion = &estado->lista_habitaciones[habitacion_actual];

    if (!habitacion->tiene_salida) {
        return;
    }

    // la salida esta en la columna ANCHO_HABITACION - 2, fila central
    int fila_salida    = ALTO_HABITACION / 2;
    int columna_salida = ANCHO_HABITACION - 2;

    if (jugador->posicion.fila == fila_salida &&
        jugador->posicion.columna == columna_salida) {
        if (jugador->tiene_objeto) {
            estado->estado_partida = ESTADO_VICTORIA;
        } else {
            printf("\nNecesitas la Llave para abrir la salida.\n");
        }
    }
}
