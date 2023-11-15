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
#include "client_worldview.h"

#define LEFT 0
#define RIGHT 1

struct client_state {
  bool is_running = false; // whether the character is currently running
  int run_phase = -1;      // run animation phase
  float position = 0.0;    // player position
  // cppcheck-suppress unusedStructMember
  unsigned int prev_ticks;
  GameState last_game_state = GameState();
};

struct client_SDL {
  // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
  SDL2pp::Window window =
      SDL2pp::Window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 480, 360, SDL_WINDOW_RESIZABLE);
  // Create accelerated video renderer with default driver
  SDL2pp::Renderer renderer =
      SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED);
  // Create resource pool
  ResourcePool resource_pool = ResourcePool(renderer);
  WorldView world_view = WorldView(resource_pool, renderer);
  SDL2pp::Texture *worm_walking = resource_pool.get_worm_walking();
};

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

public:
  explicit Client(Socket &&skt);

  void start_threads();

  void join_threads();

  int run();

  // Devuelve false si el cliente no cerro el programa, devuelve true en caso
  // contrario.
  bool func_to_execute() override;

  void handle_start_moving(int direction, bool &is_running);

  void handle_stop_moving(bool &is_running);

  void handle_finish_game();
};

#endif
