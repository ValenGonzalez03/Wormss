#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <algorithm>
#include <list>
#include <map>
#include <mutex>

#include "math.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <memory>

#include "../../common/commands/command.h"
#include "../../common/constant_rate_loop.h"
#include "../../common/game_state.h"
#include "../../common/lib/queue.h"
#include "../../common/lib/thread.h"
#include "../comm/broadcaster.h"
#include "../comm/server_sender_thread.h"
#include "../runnable_commands/command_runnable_game.h"
#include "server_game.h"

#define MAX_PLAYERS 2
#define MS_PER_UPDATE 10
// #define RATE 0.01

typedef duration<float, duration<float>> dur_ms;
typedef time_point<steady_clock, milliseconds> time_p_ms;
typedef duration<float> dur_f;

class GameManager : public Thread {
 private:
  uint8_t id;
  Game game;
  const GameConfig &config;
  Queue<game_command_ptr> commands;
  Broadcaster broadcaster;
  std::list<ServerSender *> player_senders;

  uint8_t players_counter = 0;
  uint8_t current_turn_id = 0;
  bool keep_playing = true;
  bool started = false;

  std::mutex m;
  // std::chrono::duration<float> rate =std::chrono::duration<float>((float)RATE);  // NOLINT(readability/casting)

 public:
  explicit GameManager(const uint8_t &game_id, const World &world,
                       const GameConfig &game_config);

  // Agrega el sender del jugador al broadcaster y agrega al jugador a la lista del mundo.
  void add_player(ServerSender &sender,  // NOLINT(runtime/references)
                  const uint8_t &player_id);

  // Elimina el sender del jugador del broadcaster y elimina al jugador de la lista del mundo.
  void delete_player(const uint8_t &player_id);

  // Inicializa los gusanos en el mundo y empuja el primer GameState.
  void initialize_game();

  void run() override;

  // Ejecuta un frame del juego. Actualiza el estado del mundo y las acciones de los
  // clientes, luego carga un GameState.
  // Devuelve true si el juego terminó, false en caso contrario.
  bool execute_frame();

  // Ejecuta todos los comandos de los clientes que se encontraban encolados.
  void update();

  // Crea y empuja un GameState a las Queues de los jugadores.
  void push_game_state();

  // Devuelve true si el juego terminó, false en caso contrario.
  bool has_game_finished();

  // Devuelve true si el juego fue iniciado, false en caso contrario.
  bool has_started();

  // Envia al cliente un aviso de que el juego empezó, y envía la información del mundo.
  void send_info_to_start_to_players();

  // Devuelve true si el id del juego coincide con el id recibido por parámetro,
  // false en caso contrario.
  bool compare_id(const uint8_t &another_game_id);

  // Devuelve true si el Thread terminó, false en caso contrario.
  bool is_dead();

  uint8_t get_game_id();

  Queue<game_command_ptr> *get_commands_queue();

  // GameManager &get_game_manager();

  GameManager(const GameManager &) = delete;
  GameManager &operator=(const GameManager &) = delete;
};

#endif
