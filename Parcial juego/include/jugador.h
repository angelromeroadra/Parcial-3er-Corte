#ifndef JUGADOR_H
#define JUGADOR_H

#include "tipos.h"

void mover_jugador(EstadoJuego *estado, char direccion);
void recoger_o_soltar_objeto(EstadoJuego *estado);
void revisar_salida(EstadoJuego *estado);

#endif
