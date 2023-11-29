#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <SDL2pp/SDL2pp.hh>
#include <string>
#include <utility>

#include "../common/commands/command.h"
#include "../common/constant_rate_loop.h"
#include "../common/game_state.h"
#include "../common/queue.h"
#include "client_SDL.h"
#include "client_protocol.h"
#include "client_receiver_thread.h"
#include "client_resource_pool.h"
#include "client_sender_thread.h"
#include "client_state.h"

class Client : public ConstantRateLoop {
private:
  ClientProtocol prot;

  Queue<GameState> receiver_queue;
  Queue<std::shared_ptr<Command>> sender_queue;
  ClientReceiverThread receiver;
  ClientSenderThread sender;

  std::map<int, std::function<void()>> controls;

  client_state state;

  client_SDL client_sdl;

  // Maneja el evento de comenzar a moverse al apretar
  // alguna tecla de movimiento
  void handle_start_moving(int direction, bool &is_running);

  // Maneja el evento de dejar de moverse al soltar la tecla
  // de movimiento
  void handle_stop_moving(bool &is_running);

  // Maneja el cierre del juego cuando se aprieta la 'q' o
  // la x para cerrar
  void handle_finish_game();

  // Ejecuta un evento y devuelve true si se quiere cerrar el juego
  // o false en caso contrario
  bool execute_event(SDL_Event &event);

  // Maneja el evento de saltar
  void handle_jump_forward(bool &is_jumping);

  void handle_jump_backward(bool &is_jumping);

  int get_opposite_direction();

  std::string print_state(uint8_t state);

public:
  explicit Client(ClientProtocol &&prot);

  void start_threads();

  void join_threads();

  int run();

  // Devuelve false si el cliente no cerro el programa, devuelve true en caso
  // contrario.
  bool func_to_execute() override;
};

#endif
