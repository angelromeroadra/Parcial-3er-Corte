# Profundización de Innovaciones

---

## 1. Niebla de Guerra

**¿Para qué se implementó?**
Para que el jugador explore el dungeon progresivamente sin ver el mapa completo desde el inicio, generando tensión y orientación espacial.

**¿Por qué se consideró necesario?**
En el juego de referencia (Adventure, Atari 1980) el jugador debía navegar por intuición. La niebla de guerra recrea esa experiencia sin costo de memoria: solo se añade un campo `visitada` (1 byte) por habitación.

**¿Cómo se implementó?**
Cada `Habitacion` tiene el campo `visitada`. Al entrar a una habitación se marca en `1`. En la función `dibujar_mapa_mundo`, las habitaciones con `visitada == 0` se muestran como `[ ? ]` y las visitadas muestran su número. El flag `mapa_revelado` en `EstadoJuego` permite anularlo globalmente cuando se recoge la Llave.

---

## 2. Barra de Salud Visual ASCII

**¿Para qué se implementó?**
Para dar retroalimentación visual inmediata del estado del jugador sin usar librerías gráficas externas.

**¿Por qué se consideró necesario?**
Mostrar solo un número de vida es poco intuitivo. Una barra de segmentos comunica el estado de salud de forma instantánea, igual que lo hacían los HUD de juegos clásicos de consola.

**¿Cómo se implementó?**
La función `dibujar_barra_salud` recibe `vida_actual` y `vida_maxima`. Calcula cuántos segmentos de 10 están llenos mediante una división entera y los imprime como `#` (lleno) o `-` (vacío) entre corchetes, seguido del valor numérico exacto.

---

## 3. Sistema de Turnos con Iniciativa

**¿Para qué se implementó?**
Para que el juego tenga ritmo controlado: el jugador siempre actúa antes que los enemigos, dando ventaja táctica predecible.

**¿Por qué se consideró necesario?**
Sin orden de turnos definido, los enemigos podrían moverse entre el input del jugador y su resolución, creando comportamiento impredecible. El turno explícito hace el juego justo y verificable.

**¿Cómo se implementó?**
El game loop en `main.cpp` sigue el orden estricto: (1) dibujar, (2) leer input, (3) ejecutar acción del jugador, (4) ejecutar acción de todos los enemigos. Los enemigos solo se mueven después de que el jugador ya resolvió su turno, lo que garantiza la iniciativa.

---

## 4. IA Diferenciada por Tipo de Enemigo

**¿Para qué se implementó?**
Para crear variedad táctica: el jugador debe comportarse distinto frente a un Guardia que frente a un Fantasma.

**¿Por qué se consideró necesario?**
Con un solo tipo de enemigo el juego se vuelve monótono. Dos comportamientos distintos obligan al jugador a adaptarse sin aumentar el uso de memoria (mismo `struct Enemigo`, distinto campo `tipo`).

**¿Cómo se implementó?**
`mover_guardia` elige el eje con mayor diferencia absoluta y avanza en ese eje, respetando la colisión con paredes internas. `mover_fantasma` avanza en ambos ejes simultáneamente (diagonal) y solo respeta los bordes exteriores del cuarto, simulando que "atraviesa" las paredes internas.

---

## 5. Objeto con Efecto Especial en el Mapa

**¿Para qué se implementó?**
Para que recoger la Llave tenga un efecto secundario significativo además de abrir la salida, premiando al jugador con información táctica.

**¿Por qué se consideró necesario?**
Un objeto que solo sirve de "llave" es funcionalmente trivial. Al revelar el mapa completo, el objeto adquiere valor estratégico real: el jugador debe decidir si conservarlo o soltarlo sabiendo que pierde la visión global.

**¿Cómo se implementó?**
En `recoger_o_soltar_objeto`, cuando el campo `es_llave` del objeto es `1`, se activa `estado->mapa_revelado = 1`. En `dibujar_mapa_mundo`, si `mapa_revelado` está en `1` se muestran todas las habitaciones ignorando el campo `visitada`. Al soltar la Llave, `mapa_revelado` vuelve a `0`.

---

## 6. Narrativa Ambiental por Habitación

**¿Para qué se implementó?**
Para dar contexto y atmósfera al dungeon, convirtiendo un mapa de cuadrícula en un espacio con personalidad narrativa.

**¿Por qué se consideró necesario?**
Adventure (Atari) usaba el entorno visual para contar su historia. En texto, una línea de descripción cumple el mismo rol con costo mínimo: un arreglo `char descripcion[80]` por habitación.

**¿Cómo se implementó?**
Cada `Habitacion` tiene el campo `descripcion[80]` inicializado en `inicializar_mundo`. La función `dibujar_estado` imprime la descripción de la habitación actual justo encima del mapa visual, renovándose cada vez que el jugador entra a un cuarto nuevo.

---

## 7. Estadísticas Finales de Partida

**¿Para qué se implementó?**
Para dar cierre a la partida con información que refleje el desempeño del jugador, incentivando rejugabilidad.

**¿Por qué se consideró necesario?**
Terminar con solo "ganaste" o "perdiste" es abrupto. Las estadísticas contextualizan el resultado y permiten al jugador comparar ejecuciones, algo habitual en los juegos de la era de Adventure.

**¿Cómo se implementó?**
`Jugador` acumula `pasos_dados` (incrementado en `mover_jugador`) y `enemigos_eliminados` (incrementado en `resolver_combate`). Al terminar el game loop, `mostrar_estadisticas_finales` recorre las habitaciones para contar las visitadas y presenta los tres valores junto al resultado de victoria o derrota.
