# Informe de Análisis Técnico: Proyecto Worms

## 1. Resumen Ejecutivo y Contexto
El proyecto es una implementación inspirada en el clásico videojuego multijugador por turnos **Worms** (desarrollado para la materia *Taller de Programación I - Cátedra Veiga, FIUBA*).

Está construido en **C++17** y se compone de tres ejecutables principales:
1. **`server`**: Servidor multihilo authoritative que gestiona salas/lobbies, simula el mundo con el motor de físicas [Box2D](/CMakeLists.txt#L4), ejecuta la lógica de turnos y sincroniza el estado a 60 FPS mediante un protocolo binario sobre TCP.
2. **`client`**: Cliente gráfico basado en [SDL2](/CMakeLists.txt#L5) y el wrapper moderno [SDL2pp](/CMakeLists.txt#L5), con renderizado de sprites y animaciones, control de cámara libre o centrada, reproducción de sonido/música ([`Mix_OpenAudio`](/src/client/client_client.cpp#L55)) y bucle de renderizado a tasa constante.
3. **`editor`**: Editor gráfico de niveles independiente que permite agregar, ubicar y rotar vigas (cortas y largas) y definir puntos de aparición (*spawn points*), serializando los escenarios a archivos `.yml` mediante [yaml-cpp](/CMakeLists.txt#L6).

---

## 2. Diagrama de Arquitectura y Flujo de Datos

```mermaid
flowchart TD
    subgraph Client["Cliente (SDL2 / SDL2pp)"]
        UI["Main Thread: SDL Event Loop + WorldView Render (60 FPS)"]
        CQ["sender_queue (Queue<Command>)"]
        CS["ClientSenderThread"]
        CR["ClientReceiverThread"]
        RQ["receiver_queue (Queue<GameState>)"]
        
        UI -->|Push inputs| CQ
        CQ -->|Pop bloqueante| CS
        CR -->|Push GameState| RQ
        RQ -->|Pop no-bloqueante (pop_last_one)| UI
    end

    subgraph Network["Red (TCP / Socket)"]
        CS -->|Comandos binarios| SktC2S["Socket (Client -> Server)"]
        SktS2C["Socket (Server -> Client)"] -->|GameStates binarios| CR
    end

    subgraph Server["Servidor"]
        SktC2S --> SR["ServerReceiver"]
        SR -->|try_push runnable_command| GMQ["GameManager commands Queue"]
        
        subgraph GameThread["GameManager Thread (60 FPS)"]
            GMQ -->|Execute| Loop["ConstantRateLoop (execute_frame)"]
            Loop --> Sim["ServerGame::update()\n- World::step(Box2D)\n- TurnManager::update()\n- Explosions / Rays"]
            Sim --> BC["Broadcaster"]
        end
        
        BC -->|Push GameState a cada jugador| SSQ["Sender Queue (por cliente)"]
        SSQ --> SS["ServerSenderThread"]
        SS --> SktS2C
    end
```

---

## 3. Módulos y Componentes Detallados

### 3.1. Infraestructura Común (`/src/common/`)
* **Sockets y Red ([`Socket`](/src/common/lib/socket.h), [`Resolver`](/src/common/lib/resolver.h))**: Wrapper RAII sobre sockets POSIX (`socket`, `bind`, `listen`, `accept`, `connect`, `sendall`, `recvall`, `shutdown`).
* **Concurrencia ([`Thread`](/src/common/lib/thread.h), [`Queue`](/src/common/lib/queue.h))**: Cola bloqueante multiproductor-multiconsumidor (MPMC) protegida con `std::mutex` y `std::condition_variable`, soportando cierre coordinado (`ClosedQueue`).
* **Loop a Tasa Constante ([`ConstantRateLoop`](/src/common/constant_rate_loop.h))**: Garantiza una frecuencia de actualización estable (60 FPS / `TIME_STEP = 1/60s`) compensando desviaciones temporales.
* **Estado del Juego ([`GameState`](/src/common/game_state.h))**: Estructura transferible que empaqueta las listas de gusanos ([`WormData`](/src/common/game_state.h#L17)), proyectiles/cuerpos dinámicos ([`ExplodableData`](/src/common/game_state.h#L103)) y explosiones con rayos ([`ExplosionData`](/src/common/game_state.h#L162)), además del ID del turno actual y tiempo restante. Dispone de serialización y deserialización binaria de extremo a extremo.
* **Comandos ([`Command`](/src/common/commands/command.h))**: Jerarquía de acciones serializables (`StartMoving`, `StopMoving`, `Jump`, `StartAiming`, `StopAiming`, `ChargingAttack`, `Attacking`, `ChangeWeapon`, `CreateGame`, `JoinGame`, `StartGame`, etc.).

---

### 3.2. Servidor (`/src/server/`)

#### Ciclo de Vida y Manejo de Conexiones
1. [`Server`](/src/server/server_server.h) inicializa el socket pasivo y lanza [`Accept`](/src/server/comm/server_accept_thread.h).
2. Cada conexión aceptada crea un [`ClientManager`](/src/server/comm/client_manager_thread.h):
   * En fase de Lobby atiende comandos como `CreateGame` (eligiendo nivel) o `JoinGame`.
   * Al iniciar la partida, envía la definición del escenario al cliente (`send_world`), lanza [`ServerSenderThread`](/src/server/comm/server_sender_thread.h) y convierte su propio hilo en el receptor continuo de comandos de juego ([`ServerReceiver`](/src/server/comm/server_receiver.h)).
3. [`GamesHandler`](/src/server/game/server_games_handler.h) centraliza las partidas en ejecución, recolecta hilos y partidas finalizadas (`reap_dead()`).

#### Lógica del Juego y Simulación Física
* [`GameManager`](/src/server/game/game_manager.h) ejecuta el bucle de juego independiente a 60 FPS:
  * Vacía la cola de comandos entrantes y los ejecuta polimórficamente sobre [`Game`](/src/server/game/server_game.h).
  * Avanza un paso de simulación ([`Game::update()`](/src/server/game/server_game.cpp#L49)).
  * Genera el nuevo [`GameState`](/src/server/game/server_game.cpp#L163) y lo difunde a todos los jugadores mediante [`Broadcaster`](/src/server/comm/broadcaster.h).

* [`TurnManager`](/src/server/game/turn_manager.h):
  * Maneja turnos temporizados (por defecto 10 segundos).
  * Rotación round-robin entre los jugadores vivos. Cada jugador alterna entre sus gusanos vivos ([`Player::advance_worm()`](/src/server/game/server_player.h#L27)).
  * Condición de fin de turno: El turno finaliza si el tiempo expira, o si el jugador ejecutó un ataque o recibió daño, **siempre y cuando todos los cuerpos del mundo físico hayan alcanzado el reposo** ([`World::are_all_bodies_at_rest()`](/src/server/world/server_world.cpp#L145)).

* **Motor Físico y Cuerpos ([`World`](/src/server/world/server_world.h), Box2D)**:
  * [`Body`](/src/server/bodies/body.h): Clase base que asocia un `b2Body` y `UserData`, gestionando colisiones con patrón Double Dispatch (`start_contact_with_worm`, `start_contact_with_beam`, etc.).
  * [`WormBody`](/src/server/bodies/worm_body.h): Gusano dinámico con restricción de rotación (`SetFixedRotation(true)`), sensor en los pies para detección de suelo/vigas, cálculo de daño por caída acumulando la altura máxima alcanzada (`track_max_height()`), y estados (`IDLE`, `MOVING`, `JUMPING`, `AIMING`, `CHARGING_ATTACK`, `ATTACKING`).
  * [`BeamBody`](/src/server/bodies/beam_body.h): Vigas estáticas cortas (3m) y largas (6m) con ángulos variables.
  * [`Explodable`](/src/server/bodies/explodable.h) / [`MissileBody`](/src/server/bodies/missile_body.h) / [`GrenadeBody`](/src/server/bodies/grenade_body.h): Proyectiles dinámicos que se detonan al colisionar o por temporizador (`TIME_DETONATION`).
  * [`WaterBody`](/src/server/bodies/water_body.h): Franja inferior de agua estática; cualquier contacto con un gusano aplica daño letal instantáneo.

* **Sistema de Explosiones y Ray Casting ([`Explosion`](/src/server/world/server_explosion.h))**:
  * Realiza un barrido radial con múltiples rayos (`NUM_RAYS`) mediante `b2World::RayCast`.
  * Calcula la oclusión por vigas/paredes y el punto más cercano de impacto en cada cuerpo.
  * Determina vector de impulso resultante hacia los cuerpos afectados y aplica daño inversamente proporcional a la distancia del epicentro.

---

### 3.3. Cliente (`/src/client/`)
* **Lobby inicial ([`Lobby`](/src/client/client_lobby.h))**: Menú por consola para elegir crear o unirse a una sala, listando los mapas disponibles.
* **Modelo Concurrente**:
  * **Hilo Principal**: Maneja eventos de teclado/mouse SDL, renderiza a 60 FPS y extrae el último `GameState` disponible mediante `pop_last_one()`.
  * **[`ClientSenderThread`](/src/client/comm/client_sender_thread.h)**: Envía los comandos encolados de manera asíncrona hacia el socket.
  * **[`ClientReceiverThread`](/src/client/comm/client_receiver_thread.h)**: Recibe continuamente los `GameState` desde el servidor y los deposita en la cola.
* **Renderizado y Recursos**:
  * [`ResourcePool`](/src/client/client_resource_pool.h): Carga y cachea texturas de sprites (animaciones de movimiento, apuntado, carga de disparo), fuentes TTF y pistas de audio.
  * [`WorldView`](/src/client/view/client_worldview.h): Traduce el estado del juego a píxeles en pantalla. Realiza la conversión de coordenadas entre el origen inferior-izquierdo de Box2D y el superior-izquierdo de SDL.
  * [`Camera`](/src/client/view/client_camera.h): Soporta modo de seguimiento automático centrado en el gusano activo del turno, o modo libre desplazándose con el cursor en los bordes de la ventana.

---

### 3.4. Editor de Niveles (`/level_editor/`)
* Permite crear escenarios interactivos desde cero o modificar existentes.
* Controles:
  * `s + click izquierdo`: Agrega viga corta.
  * `l + click izquierdo`: Agrega viga larga.
  * `w + click izquierdo`: Agrega punto de spawn.
  * `z`: Deshace la última viga.
  * `x`: Deshace el último spawn point.
* [`FileEditor`](/level_editor/file_editor.h): Almacena y recupera los niveles en formato YAML estándar compatible con el `WorldsReader` del servidor.

---

## 4. Hallazgos Técnicos y Áreas Notables de Mejora

Alineado con las notas encontradas en [/docs/Cosas a corregir.txt](/docs/Cosas%20a%20corregir.txt) y el análisis del código:

1. **Bug del Misil que atraviesa al gusano sin explotar**:
   * **Causa Física (Tunneling)**: Proyectiles rápidos impulsados a alta velocidad (`BLAST_POWER = 60`) pueden atravesar cuerpos de dimensiones reducidas (ancho del gusano: 0.8m) entre dos pasos discretos de Box2D si no está activado el *Continuous Collision Detection* (CCD). Es necesario configurar `bodyDef.bullet = true` en proyectiles dinámicos.
   * **Causa en Spawn**: En [`ServerBazooka::attack`](/src/server/game/weapons/server_bazooka.cpp#L12), el callback [`MissileCallback`](/src/server/world/trajectory_missile_callback.cpp#L10) solo detecta colisiones con `BEAM`, ignorando completamente cuerpos tipo `WORM` al verificar el punto de aparición a corta distancia.
2. **Cálculo del daño de la granada atado a la carga**:
   * En [`World::update_explodables()`](/src/server/world/server_world.cpp#L122), se invoca `create_explosion` pasando `charge_intensity`. En granadas, la carga representa la fuerza con la que se arrojó la granada, no la potencia explosiva, por lo que una granada arrojada suavemente produce una explosión con impulso casi nulo.
3. **Invalidación de Iteradores en `WorldView::update`**:
   * En [`WorldView::update`](/src/client/view/client_worldview.cpp#L174), se ejecuta `explodables.erase(explodable.first)` mientras se itera sobre el mismo `std::map<uint8_t, Explodable>` usando un bucle range-for, lo que provoca comportamiento indefinido (iterator invalidation).
4. **Acoplamiento en Callbacks**:
   * [`BaseballBatCallback`](/src/server/world/baseball_bat_callback.cpp#L17) aplica directamente daño y fuerzas sobre el cuerpo impactado dentro del callback de Box2D en lugar de delegar la resolución a [`ServerBat`](/src/server/game/weapons/server_bat.cpp).
5. **Comandos y RunnableCommands**:
   * El uso de `reinterpret_cast` en clases derivadas de [`RunnableCommandGame`](/src/server/runnable_commands/attacking_runnable.h#L16) para acceder a los datos de comandos concretos puede sustituirse limpiamente con métodos virtuales en `Command` o mediante composición con los protocolos.
