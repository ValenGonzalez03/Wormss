#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <SDL2pp/SDL2pp.hh>
#include <string>
#include <utility>

#include "../common/constant_rate_loop.h"
#include "../common/game_state.h"
#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "client_receiver_thread.h"
#include "client_resource_pool.h"
#include "client_sender_thread.h"
#include "client_state.h"
#include "client_SDL.h"

class Client : public ConstantRateLoop {
private:
  Protocol prot;

  Queue<GameState> receiver_queue;
  Queue<std::shared_ptr<Command>> sender_queue;
  ClientReceiverThread receiver;
  ClientSenderThread sender;

  std::map<int, std::function<void()>> controls;

  client_state state;

  client_SDL client_sdl;

  void handle_start_moving(int direction, bool &is_running);

  void handle_stop_moving(bool &is_running);

  void handle_finish_game();

  // Ejecuta un evento y devuelve true si se quiere cerrar el juego
  // o false en caso contrario
  bool execute_event(SDL_Event &event);

public:
  explicit Client(Socket &&skt);

  void start_threads();

  void join_threads();

  int run();

  // Devuelve false si el cliente no cerro el programa, devuelve true en caso
  // contrario.
  bool func_to_execute() override;

  // Convierte una posicion en metros a una posicion en pixeles.
  //Position convert_to_pixels(Position &pos);

};

#endif
