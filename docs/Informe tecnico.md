# Informe técnico sobre el tp

## Introducción

Este informe tratará sobre el lado técnico del tp, es decir, la explicación del modelo adoptado, la cantidad y tareas de los hilos, la comunicación entre cliente y servidor y cosas afines.

## Cliente

### Clases cliente

## Servidor

### Clases servidor

## Clases comunes

## Protocolo

La comunicación entre el cliente y servidor comienza en el lobby, con el envío de 'comandos de lobby', estos son: CreateGame, JoinGame y StartGame.

Al enviar el CreateGame desde el cliente hacia el servidor, éste último responde enviando el player_id, el game_id y los nombres de los niveles disponibles; el cliente selecciona uno y lo envía por socket.

Si en vez de enviar CreateGame, se envía JoinGame, el cliente debe enviar el game_id al que se quiere unir (mientras el servidor espera), una vez enviado, recibe el player_id.

Luego de que el creador de la partida envía el comando StartGame, el/los cliente/s que se hayan unido a la partida reciben el nivel completo, es decir, el nombre del nivel, nombre del background, posición de las vigas, etc. El cliente lo carga y comienza el juego.

Una vez que se recibió el nivel, se inicializa SDL y se abren los hilos Sender y Receiver del cliente, mientras que con el hilo principal se ejecuta el main loop.

El hilo sender es el encargado del envío de comandos hacia el servidor, para esto se desarrollaron diferentes códigos que representan los diferentes comandos. Estos son:

```cpp
uint8_t CREATE_GAME = 0x01;
uint8_t JOIN_GAME = 0x02;
uint8_t START_GAME = 0x03;
uint8_t GAME_STARTED = 0x04;
uint8_t START_MOVING = 0x05;
uint8_t STOP_MOVING = 0x06;
uint8_t JUMP = 0x07;
uint8_t BACK_JUMP = 0x08;
uint8_t START_AIMING = 0x09;
uint8_t STOP_AIMING = 0x0A;
uint8_t START_SHOOTING = 0x0B;
uint8_t STOP_SHOOTING = 0x0C;
```

Mientras tanto el hilo Receiver (del cliente) recibe GameStates.

Tanto los comandos como los gamestates tienen la particularidad de que saben como enviarse y recibirse por el socket, lo que provee la ventaja de la adición de nuevas funcionalidades sin modificar demasiado el código.


## Comentarios finales

Algunas partes importantes a destacar en el desarrollo del tp:

- Cambio de ejes entre el servidor y el cliente: Dado que Box2D implementa un trazado de ejes con el origen centrado en la esquina inferior izquierda, mientras que SDL2 contiene su origen en la esquina superior derecha, se tuvo que recurrir a una transformación para que coincidan las coordenadas, así como también un mapeo entre metros (utilizados por Box2D) y píxeles (utilizados por SDL2).

- Utilización de nuevas herramientas: se tuvo que proceder a aprender y saber utilizar bibliotecas tales como SDL2 y Box2D; aplicar lo aprendido sobre sockets, threads, namespaces, excepciones, etc.
Creemos que todo esto nos ayudó a mejorar y plantear de mejor manera proyectos a futuro.



![img.png](client_structure.png)