#ifndef TIPOS_H
#define TIPOS_H

// -- bueno pues... defino las constantes globales del mundo --

#define FILAS_MAPA        5
#define COLUMNAS_MAPA     5
#define MAXIMO_ENEMIGOS   8
#define MAXIMO_OBJETOS    4
#define MAXIMO_HABITACIONES 6
#define ANCHO_HABITACION  9
#define ALTO_HABITACION   5

// -- a ver... tipos de celda para el mapa visual --
#define CELDA_VACIA      ' '
#define CELDA_PARED      '#'
#define CELDA_JUGADOR    '@'
#define CELDA_GUARDIA    'G'
#define CELDA_FANTASMA   'F'
#define CELDA_OBJETO     '*'
#define CELDA_SALIDA     'E'
#define CELDA_SUELO      '.'

// -- estados posibles de la partida --
#define ESTADO_JUGANDO   0
#define ESTADO_VICTORIA  1
#define ESTADO_DERROTA   2

// -- tipos de enemigo --
#define TIPO_GUARDIA     0
#define TIPO_FANTASMA    1

struct Posicion {
    int fila;
    int columna;
};

struct Enemigo {
    Posicion posicion;
    int tipo;
    int puntos_de_vida;
    int esta_vivo;
    int habitacion_actual;
};

struct Objeto {
    Posicion posicion;
    int habitacion_actual;
    int recogido;
    char nombre[20];
    int es_llave;
};

struct Habitacion {
    int identificador;
    int norte;    // -1 si no hay conexion
    int sur;
    int este;
    int oeste;
    char descripcion[80];
    int visitada;
    int tiene_salida;
    // mapa interno de la habitacion
    char cuadricula[ALTO_HABITACION][ANCHO_HABITACION];
};

struct Jugador {
    Posicion posicion;
    int puntos_de_vida;
    int puntos_de_vida_maximo;
    int tiene_objeto;
    int habitacion_actual;
    int pasos_dados;
    int enemigos_eliminados;
};

struct EstadoJuego {
    Jugador jugador;
    Enemigo lista_enemigos[MAXIMO_ENEMIGOS];
    Objeto lista_objetos[MAXIMO_OBJETOS];
    Habitacion lista_habitaciones[MAXIMO_HABITACIONES];
    int cantidad_enemigos;
    int cantidad_objetos;
    int estado_partida;
    int mapa_revelado;
};

#endif
