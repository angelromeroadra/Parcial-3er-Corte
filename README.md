Dungeon Crawler - Parcial Final

Autor: Angel David Romero Alarcon


## Innovaciones implementadas

1. **Niebla de Guerra** — Solo se revelan en el mapa las habitaciones ya visitadas por el jugador.
2. **Barra de salud visual ASCII** — La vida del jugador se muestra con caracteres `#` y `-` que cambian en tiempo real.
3. **Sistema de turnos con iniciativa** — Jugador y enemigos actúan en orden estricto: input → jugador → enemigos.
4. **IA diferenciada por tipo de enemigo** — El Guardia persigue en un solo eje respetando paredes; el Fantasma se mueve en diagonal atravesando obstáculos internos.
5. **Objeto con efecto especial en el mapa** — Recoger la Llave revela instantáneamente todas las habitaciones; soltarla vuelve a ocultarlas.
6. **Narrativa ambiental por habitación** — Cada cuarto tiene una descripción de texto única que aparece al entrar.
7. **Estadísticas finales de partida** — Al ganar o perder se muestran pasos dados, enemigos eliminados y habitaciones exploradas.



## Cómo compilar

Linux / Mac


mkdir build
cd build
cmake ..
make
./dungeon


Windows (MinGW o MSVC)


mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
dungeon.exe


## Compilación directa sin CMake


g++ -std=c++11 -Iinclude main.cpp src/mundo.cpp src/render.cpp src/jugador.cpp src/enemigos.cpp -o dungeon
./dungeon




Controles

| Tecla | Acción          |
|-------|-----------------|
| W     | Mover arriba    |
| S     | Mover abajo     |
| A     | Mover izquierda |
| D     | Mover derecha   |
| E     | Recoger / Soltar objeto |
| Q     | Salir del juego |
