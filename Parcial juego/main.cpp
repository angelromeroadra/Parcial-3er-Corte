#include <stdio.h>
#include <string.h>

#include "include/tipos.h"
#include "include/mundo.h"
#include "include/render.h"
#include "include/jugador.h"
#include "include/enemigos.h"

// -- listo ya... el game loop principal con sistema de turnos (innovacion 3) --
int main() {
    EstadoJuego estado;
    char entrada[4];
    char tecla;

    inicializar_mundo(&estado);

    while (estado.estado_partida == ESTADO_JUGANDO) {
        // TURNO 1: dibujar estado actual
        dibujar_estado(&estado);

        // TURNO 2: leer input del jugador
        printf("\nAccion: ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            break;
        }
        tecla = entrada[0];

        if (tecla == 'q' || tecla == 'Q') {
            printf("\nAbandonaste el dungeon.\n");
            break;
        }

        // TURNO 3: accion del jugador
        if (tecla == 'w' || tecla == 'W' ||
            tecla == 'a' || tecla == 'A' ||
            tecla == 's' || tecla == 'S' ||
            tecla == 'd' || tecla == 'D') {
            mover_jugador(&estado, tecla);
            revisar_salida(&estado);
        } else if (tecla == 'e' || tecla == 'E') {
            recoger_o_soltar_objeto(&estado);
            revisar_salida(&estado);
        }

        if (estado.estado_partida != ESTADO_JUGANDO) {
            break;
        }

        // TURNO 4: accion de los enemigos
        actualizar_enemigos(&estado);
    }

    // mostrar pantalla final con estadisticas (innovacion 7)
    limpiar_pantalla();
    mostrar_estadisticas_finales(&estado);

    return 0;
}
