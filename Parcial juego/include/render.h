#ifndef RENDER_H
#define RENDER_H

#include "tipos.h"

void limpiar_pantalla();
void dibujar_estado(EstadoJuego *estado);
void dibujar_habitacion(EstadoJuego *estado);
void dibujar_barra_salud(int vida_actual, int vida_maxima);
void dibujar_mapa_mundo(EstadoJuego *estado);
void mostrar_estadisticas_finales(EstadoJuego *estado);

#endif
