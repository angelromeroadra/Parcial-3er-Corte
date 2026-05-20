#include "../include/mundo.h"
#include <string.h>

// -- oye mira... construyo la cuadricula base de cada habitacion --
void construir_habitacion(Habitacion *habitacion) {
    int fila;
    int columna;

    for (fila = 0; fila < ALTO_HABITACION; fila++) {
        for (columna = 0; columna < ANCHO_HABITACION; columna++) {
            if (fila == 0 || fila == ALTO_HABITACION - 1 ||
                columna == 0 || columna == ANCHO_HABITACION - 1) {
                habitacion->cuadricula[fila][columna] = CELDA_PARED;
            } else {
                habitacion->cuadricula[fila][columna] = CELDA_SUELO;
            }
        }
    }

    // -- puertas segun conexiones --
    if (habitacion->norte != -1) {
        habitacion->cuadricula[0][ANCHO_HABITACION / 2] = CELDA_SUELO;
    }
    if (habitacion->sur != -1) {
        habitacion->cuadricula[ALTO_HABITACION - 1][ANCHO_HABITACION / 2] = CELDA_SUELO;
    }
    if (habitacion->este != -1) {
        habitacion->cuadricula[ALTO_HABITACION / 2][ANCHO_HABITACION - 1] = CELDA_SUELO;
    }
    if (habitacion->oeste != -1) {
        habitacion->cuadricula[ALTO_HABITACION / 2][0] = CELDA_SUELO;
    }

    // -- marcar salida si aplica --
    if (habitacion->tiene_salida) {
        habitacion->cuadricula[ALTO_HABITACION / 2][ANCHO_HABITACION - 2] = CELDA_SALIDA;
    }
}

// -- bueno entonces... configuro las 6 habitaciones del dungeon --
void inicializar_mundo(EstadoJuego *estado) {
    int indice;

    // ---- HABITACIONES ----
    // Habitacion 0: Sala de entrada
    estado->lista_habitaciones[0].identificador = 0;
    estado->lista_habitaciones[0].norte = -1;
    estado->lista_habitaciones[0].sur  = 2;
    estado->lista_habitaciones[0].este = 1;
    estado->lista_habitaciones[0].oeste = -1;
    estado->lista_habitaciones[0].tiene_salida = 0;
    estado->lista_habitaciones[0].visitada = 0;
    strcpy(estado->lista_habitaciones[0].descripcion,
           "La entrada. Huele a piedra vieja.");
    construir_habitacion(&estado->lista_habitaciones[0]);

    // Habitacion 1: Corredor este
    estado->lista_habitaciones[1].identificador = 1;
    estado->lista_habitaciones[1].norte = -1;
    estado->lista_habitaciones[1].sur  = 3;
    estado->lista_habitaciones[1].este = -1;
    estado->lista_habitaciones[1].oeste = 0;
    estado->lista_habitaciones[1].tiene_salida = 0;
    estado->lista_habitaciones[1].visitada = 0;
    strcpy(estado->lista_habitaciones[1].descripcion,
           "Un corredor estrecho. Hay antorchas apagadas.");
    construir_habitacion(&estado->lista_habitaciones[1]);

    // Habitacion 2: Camara sur
    estado->lista_habitaciones[2].identificador = 2;
    estado->lista_habitaciones[2].norte = 0;
    estado->lista_habitaciones[2].sur  = 4;
    estado->lista_habitaciones[2].este = 3;
    estado->lista_habitaciones[2].oeste = -1;
    estado->lista_habitaciones[2].tiene_salida = 0;
    estado->lista_habitaciones[2].visitada = 0;
    strcpy(estado->lista_habitaciones[2].descripcion,
           "Camara oscura. Algo se mueve en las sombras.");
    construir_habitacion(&estado->lista_habitaciones[2]);

    // Habitacion 3: Cruce central
    estado->lista_habitaciones[3].identificador = 3;
    estado->lista_habitaciones[3].norte = 1;
    estado->lista_habitaciones[3].sur  = 5;
    estado->lista_habitaciones[3].este = -1;
    estado->lista_habitaciones[3].oeste = 2;
    estado->lista_habitaciones[3].tiene_salida = 0;
    estado->lista_habitaciones[3].visitada = 0;
    strcpy(estado->lista_habitaciones[3].descripcion,
           "Cruce del dungeon. Los pasos resuenan.");
    construir_habitacion(&estado->lista_habitaciones[3]);

    // Habitacion 4: Sala del tesoro
    estado->lista_habitaciones[4].identificador = 4;
    estado->lista_habitaciones[4].norte = 2;
    estado->lista_habitaciones[4].sur  = -1;
    estado->lista_habitaciones[4].este = 5;
    estado->lista_habitaciones[4].oeste = -1;
    estado->lista_habitaciones[4].tiene_salida = 0;
    estado->lista_habitaciones[4].visitada = 0;
    strcpy(estado->lista_habitaciones[4].descripcion,
           "Sala del tesoro. La llave brilla en el suelo.");
    construir_habitacion(&estado->lista_habitaciones[4]);

    // Habitacion 5: Camara de salida
    estado->lista_habitaciones[5].identificador = 5;
    estado->lista_habitaciones[5].norte = 3;
    estado->lista_habitaciones[5].sur  = -1;
    estado->lista_habitaciones[5].este = -1;
    estado->lista_habitaciones[5].oeste = 4;
    estado->lista_habitaciones[5].tiene_salida = 1;
    estado->lista_habitaciones[5].visitada = 0;
    strcpy(estado->lista_habitaciones[5].descripcion,
           "La salida esta cerca. Se siente el viento.");
    construir_habitacion(&estado->lista_habitaciones[5]);

    // ---- JUGADOR ----
    estado->jugador.posicion.fila    = 2;
    estado->jugador.posicion.columna = 2;
    estado->jugador.puntos_de_vida         = 10;
    estado->jugador.puntos_de_vida_maximo  = 10;
    estado->jugador.tiene_objeto    = 0;
    estado->jugador.habitacion_actual = 0;
    estado->jugador.pasos_dados      = 0;
    estado->jugador.enemigos_eliminados = 0;

    // ---- ENEMIGOS ----
    // Guardia 1 - habitacion 1
    estado->lista_enemigos[0].tipo = TIPO_GUARDIA;
    estado->lista_enemigos[0].posicion.fila    = 2;
    estado->lista_enemigos[0].posicion.columna = 4;
    estado->lista_enemigos[0].puntos_de_vida   = 3;
    estado->lista_enemigos[0].esta_vivo        = 1;
    estado->lista_enemigos[0].habitacion_actual = 1;

    // Guardia 2 - habitacion 3
    estado->lista_enemigos[1].tipo = TIPO_GUARDIA;
    estado->lista_enemigos[1].posicion.fila    = 3;
    estado->lista_enemigos[1].posicion.columna = 3;
    estado->lista_enemigos[1].puntos_de_vida   = 3;
    estado->lista_enemigos[1].esta_vivo        = 1;
    estado->lista_enemigos[1].habitacion_actual = 3;

    // Guardia 3 - habitacion 5
    estado->lista_enemigos[2].tipo = TIPO_GUARDIA;
    estado->lista_enemigos[2].posicion.fila    = 1;
    estado->lista_enemigos[2].posicion.columna = 4;
    estado->lista_enemigos[2].puntos_de_vida   = 3;
    estado->lista_enemigos[2].esta_vivo        = 1;
    estado->lista_enemigos[2].habitacion_actual = 5;

    // Fantasma 1 - habitacion 2
    estado->lista_enemigos[3].tipo = TIPO_FANTASMA;
    estado->lista_enemigos[3].posicion.fila    = 2;
    estado->lista_enemigos[3].posicion.columna = 5;
    estado->lista_enemigos[3].puntos_de_vida   = 2;
    estado->lista_enemigos[3].esta_vivo        = 1;
    estado->lista_enemigos[3].habitacion_actual = 2;

    // Fantasma 2 - habitacion 4
    estado->lista_enemigos[4].tipo = TIPO_FANTASMA;
    estado->lista_enemigos[4].posicion.fila    = 3;
    estado->lista_enemigos[4].posicion.columna = 2;
    estado->lista_enemigos[4].puntos_de_vida   = 2;
    estado->lista_enemigos[4].esta_vivo        = 1;
    estado->lista_enemigos[4].habitacion_actual = 4;

    estado->cantidad_enemigos = 5;

    // ---- OBJETOS ----
    // Llave - habitacion 4
    estado->lista_objetos[0].posicion.fila    = 1;
    estado->lista_objetos[0].posicion.columna = 6;
    estado->lista_objetos[0].habitacion_actual = 4;
    estado->lista_objetos[0].recogido = 0;
    estado->lista_objetos[0].es_llave = 1;
    strcpy(estado->lista_objetos[0].nombre, "Llave");

    estado->cantidad_objetos = 1;

    // ---- ESTADO INICIAL ----
    estado->estado_partida = ESTADO_JUGANDO;
    estado->mapa_revelado  = 0;

    // marcar habitacion inicial como visitada
    estado->lista_habitaciones[0].visitada = 1;

    // poner al jugador en la cuadricula inicial
    for (indice = 0; indice < MAXIMO_HABITACIONES; indice++) {
        // reset visual - se hace en render al dibujar
    }
}
