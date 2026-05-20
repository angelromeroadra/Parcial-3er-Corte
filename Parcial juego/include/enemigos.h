#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include "tipos.h"

void actualizar_enemigos(EstadoJuego *estado);
void mover_guardia(EstadoJuego *estado, Enemigo *enemigo);
void mover_fantasma(EstadoJuego *estado, Enemigo *enemigo);
void resolver_combate(EstadoJuego *estado);

#endif
