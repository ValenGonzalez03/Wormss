#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>
#include <string>
#include <utility>
#include <set>
#include <memory>

#include "../common/commands/command.h"
#include "../common/constant_rate_loop.h"
#include "../common/game_state.h"
#include "../common/lib/queue.h"
#include "client_view.h"
#include "client_resource_pool.h"
#include "comm/client_protocol.h"
#include "comm/client_receiver_thread.h"
#include "comm/client_sender_thread.h"

// #define MAX_CHARGE 15.0f
// #define CHARGE_RATE 0.15f
#define BAT_CHARGE 8.0f

static constexpr float MAX_CHARGE = 20.0f;
static constexpr float CHARGE_RATE = 0.15f;


class Client {
 private:
  ClientProtocol prot;

  Queue<GameState> receiver_queue;
  Queue<std::shared_ptr<Command>> sender_queue;
  ClientReceiverThread receiver;
  ClientSenderThread sender;
  bool keep_playing = true;

  GameState last_game_state;

  ClientView view;

  uint8_t player_id;

  std::set<int> mov_keys_pressed;
  std::set<int> aim_keys_pressed;

  bool is_charging_attack = false;
  float charge_power = 0.0f;

  void recv_world();

  // Ejecuta un evento y devuelve true si se quiere cerrar el juego o false en caso contrario
  bool execute_event(SDL_Event &event);  // NOLINT(runtime/references)

  // Maneja el evento de comenzar a moverse al apretar alguna tecla de movimiento
  void handle_start_moving(int direction);

  // Maneja el evento de dejar de moverse al soltar la tecla de movimiento
  void handle_stop_moving();

  // Maneja la finalizacion del juego cuando se aprieta ESCAPE.
  void handle_stop_game();

  // Maneja el cierre del juego (Unicamente por parte del cliente en cuestión) cuando se aprieta la 'q' o la x para cerrar
  void handle_quit_game();

  // Maneja el evento de saltar hacia adelante
  void handle_jump_forward(uint8_t worm_dir, uint8_t jump_type);

  // Maneja el evento de saltar hacia atrás
  void handle_jump_backward(uint8_t worm_dir, uint8_t jump_type);

  // Maneja el evento de comienzo de apuntado
  void handle_start_aiming(int direction);

  // Maneja el evento de finalización de apuntado
  void handle_stop_aiming();

  // Maneja el evento de carga de ataque (Para armas que requieran carga)
  void handle_charging_attack();

  // Maneja el evento de ataque con la potencia correspondiente
  void handle_attacking(float charge_power);

  // Maneja el evento de cambio de arma
  void handle_change_weapon(uint8_t weapon_type);

  // Muestra el estado del gusano
  std::string print_state(WormState state);

 public:
  explicit Client(ClientProtocol &&prot, uint8_t player_id);

  void start_threads();

  void join_threads();

  int run();

  // Devuelve false si el cliente no cerro el programa, devuelve true en caso contrario.
  bool execute_frame();
};

#endif
