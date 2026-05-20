Profundización de Innovaciones

1. Niebla de Guerra
¿Para qué se implementó?
Se implementó para que el jugador descubra el dungeon de manera gradual sin poder ver el mapa en su totalidad desde el principio, creando así tensión y mejorando la orientación espacial.
¿Por qué se consideró necesario?
En el juego de referencia (Adventure, Atari 1980), el jugador tenía que orientarse a través de la intuición. La niebla de guerra reproduce esta experiencia sin costo de memoria adicional: solo se agrega un campo visitada (1 byte) por cada habitación.
¿Cómo se implementó?
Cada Habitacion posee el campo visitada. Al ingresar a una habitación, se marca como 1. En la función dibujar_mapa_mundo, las habitaciones con visitada == 0 se representan como [ ? ], mientras que las visitadas muestran su número. El flag mapa_revelado en EstadoJuego permite desactivarlo globalmente al recoger la Llave.

2. Barra de Salud Visual ASCII
¿Para qué se implementó?
Se implementó para proporcionar al jugador una retroalimentación visual inmediata sobre su estado sin necesidad de utilizar librerías gráficas externas.
¿Por qué se consideró necesario?
Simplemente mostrar un número de vida resulta poco intuitivo. Una barra de segmentos permite comunicar instantáneamente el estado de salud, de manera similar a los HUD de los juegos clásicos de consolas.
¿Cómo se implementó?
La función dibujar_barra_salud recibe vida_actual y vida_maxima. Calcula cuántos segmentos de 10 están llenos mediante una división entera y los muestra como # (lleno) o - (vacío) entre corchetes, seguido del valor numérico exacto.

3. Sistema de Turnos con Iniciativa
¿Para qué se implementó?
Se implementó para que el juego mantenga un ritmo controlado: el jugador siempre actúa antes que los enemigos, lo que proporciona una ventaja táctica predecible.
¿Por qué se consideró necesario?
Sin un orden de turnos definido, los enemigos podrían moverse entre la entrada del jugador y su resolución, creando comportamientos impredecibles. El turno explícito hace que el juego sea justo y verificable.
¿Cómo se implementó?
El ciclo de juego en main.cpp sigue un orden estricto: (1) dibujar, (2) leer la entrada, (3) ejecutar la acción del jugador, (4) ejecutar la acción de todos los enemigos. Los enemigos solo se mueven después de que el jugador ha resuelto su turno, garantizando así la iniciativa.

4. IA Diferenciada por Tipo de Enemigo
¿Para qué se implementó?
Se implementó para introducir variedad táctica: el jugador debe actuar de manera diferente frente a un Guardia en comparación con un Fantasma.
¿Por qué se consideró necesario?
Si solo existiera un tipo de enemigo, el juego se volvería monótono. Tener dos comportamientos distintos obliga al jugador a adaptarse sin aumentar el uso de memoria (mismo struct Enemigo, distinto campo tipo).
¿Cómo se implementó?
mover_guardia elige el eje con la mayor diferencia absoluta y se desplaza en ese eje, respetando la colisión con paredes internas. Por otro lado, mover_fantasma se mueve en ambos ejes simultáneamente (diagonal) y solo respeta los bordes externos de la habitación, simulando que "atraviesa" las paredes internas.

5. Objeto con Efecto Especial en el Mapa
¿Para qué se implementó?
Se implementó para que la recolección de la Llave tenga un efecto secundario importante, además de abrir la salida, brindando al jugador información táctica.
¿Por qué se consideró necesario?
Un objeto que solo actúa como "llave" es funcionalmente trivial. Al revelar el mapa completo, el objeto obtiene un valor estratégico real: el jugador debe decidir si mantenerlo o soltarlo, sabiendo que perderá la visión global.
¿Cómo se implementó?
En recoger_o_soltar_objeto, cuando el campo es_llave del objeto es 1, se activa estado->mapa_revelado = 1. En dibujar_mapa_mundo, si mapa_revelado está en 1, se muestran todas las habitaciones ignorando el campo visitada. Al soltar la Llave, mapa_revelado se restablece a 0.

6. Narrativa Ambiental por Habitación
¿Para qué se implementó?
Se implementó para proporcionar contexto y atmósfera al dungeon, transformando un mapa de cuadrículas en un espacio con personalidad narrativa.
¿Por qué se consideró necesario?
Adventure (Atari) utilizaba el entorno visual para contar su historia. En formato de texto, una línea de descripción cumple la misma función con un costo mínimo: un arreglo char descripcion[80] por habitación.
¿Cómo se implementó?
Cada Habitacion cuenta con el campo descripcion[80] inicializado en inicializar_mundo. La función dibujar_estado imprime la descripción de la habitación actual justo encima del mapa visual, actualizándose cada vez que el jugador entra en un nuevo cuarto.

7. Estadísticas Finales de Partida
¿Para qué se implementó?
Se implementó para concluir la partida proporcionando información que refleje el desempeño del jugador, incentivando así la rejugabilidad.
¿Por qué se consideró necesario?
Terminar con un simple "ganaste" o "perdiste" resulta abrupto. Las estadísticas dan contexto al resultado y permiten al jugador comparar sus partidas, algo común en los juegos de la era de Adventure.
¿Cómo se implementó?
Jugador acumula pasos_dados (incrementado en mover_jugador) y enemigos_eliminados (incrementado en resolver_combate). Al finalizar el ciclo de juego, mostrar_estadisticas_finales recorre las habitaciones para contar las visitadas y presenta estos tres valores junto al resultado de victoria o derrota.
