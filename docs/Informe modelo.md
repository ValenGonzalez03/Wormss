# Informe

## Introducción

En este trabajo práctico desarrollamos una semejanza al famoso juego "Worms". Para ello tuvimos que desarrollar la estructura y conexión entre cliente y servidor, así como también la comunicación entre los mismos, el renderizado de las imágenes, la implementación de las físicas y la creación del archivo cmake para compilar.

Para el renderizado de las texturas y la reproducción de sonidos utilizamos la biblioteca SDL2.

Para la implementación de las físicas utilizamos Box2D.

Para la lectura de archivos de niveles y configuración utilizamos la librería de lectura y escritura de archivos YAML.

## Cliente

El cliente posee 2 partes:

* Lobby

* Juego

**Lobby:** Se trata de la "sala de espera" a la que accede un cliente/jugador al conectarse. En ella se puede elegir 2 opciones:

- Crear una nueva partida: en cuyo caso elegirá el escenario deseado, lo enviará al server, recibirá el game_id de la partida creada y deberá esperar a que los demás jugadores se unan. Una vez que se hayan unido comienza la partida presionando la 's'.

- Unirse a una partida: deberá introducir el game_id de la partida a la cual se quiere unir y una vez hecho esto, esperar a que el creador de la partida la inicie presionando 's'.

**Juego:** Una vez unido el jugador/jugadores se abrirá una ventana que los mostrará y se establecerá la conexión con el server y entre ellos, reflejado en el juego.

## Servidor

Del lado del servidor se ejecutarán las acciones necesarias para que el juego funcione, esto es:

- La recepción y procesamiento de comandos del cliente
- La ejecución de las físicas del juego
- La devolución del estado del juego a los jugadores de la partida

Cabe destacar que el servidor también es el encargado de la aceptación de nuevos jugadores, la conexión entre clientes en una partida, la lectura de los niveles y configuraciones.

## Comandos o controles del juego

La siguiente es una lista de controles del juego:

`→` Moverse hacia la derecha

`←` Moverse hacia la izquierda

`↓` Apuntar hacia abajo

`↑` Apuntar hacia arriba

`enter` Saltar hacia adelante

`backspace` Saltar hacia atrás

`i` Bajar volúmen música

`o` Subir volúmen música

## Editor de niveles

El editor de niveles es una herramienta que permite la creación de nuevos niveles o edición de niveles ya creados. Este posee un lobby donde nos preguntará si deseamos crear o editar un nivel y luego nos redigirá al editor gráfico (creado con SDL2). En él podremos agregar o quitar vigas largas/cortas y puntos de aparición.

### Controles del editor

Los controles del editor se muestran en la ventana gráfica pero igualmente los desarrollaremos aquí:

`s + left click` Coloca una viga corta en la posición del puntero

`l + left click` Coloca una viga larga en la posición del puntero

`w + left click` Coloca un punto de aparición (spawn point) en la posición del puntero

`z` Elimina la última viga colocada

`x` Elimina el último spawn point colocado