#include "client_client.h"

#include <list>

#include "../common/commands/create_game.h"
#include "../common/commands/join_game.h"
#include "../common/commands/jump.h"
#include "../common/commands/start_aiming.h"
#include "../common/commands/start_game.h"
#include "../common/commands/start_moving.h"
#include "../common/commands/stop_aiming.h"
#include "../common/commands/stop_moving.h"
#include "../common/worm_states.h"

using namespace SDL2pp;

const float RATE = (float)(1.0 / 60.0);

Client::Client(ClientProtocol &&prot, uint8_t player_id)
    : prot(std::move(prot)), receiver_queue(), sender_queue(),
      receiver(this->prot, receiver_queue), sender(this->prot, sender_queue),
      state(), client_sdl(), player_id(player_id) {}

void Client::start_threads() {
  sender.start();
  receiver.start();
}

void Client::join_threads() {
  sender.join();
  receiver.join();
}

int Client::run() {

  // Initialize SDL library
  SDL sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  // Initialize SDL_ttf library
  SDLTTF ttf;

  // Initialize SDLMIXER library
  SDLMixer mixer;
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
      throw std::runtime_error("SDL_mixer could not initialize!");
  }

  // INICIALIZO LA RESOURCE POOL (LAS TEXTURAS, MUSICA, ETC)
  client_sdl.resource_pool.initialize();
  client_sdl.resource_pool.play_music();


  // RECIBO EL MUNDO DEL SERVER
  bool was_closed = false;
  prot.recv_world(client_sdl.world_view, &was_closed);

  state.prev_ticks = SDL_GetTicks();

  start_threads();

  // Loop del ConstantRateLoop, recibe como parametro el rate, que determina
  // cuantos frames se renderizan en un segundo

  loop(std::chrono::duration<float>((float)RATE));

  return 0;
}


bool Client::func_to_execute() {
  // Timing: calculate difference between this and previous frame
  // in milliseconds
  unsigned int frame_ticks = SDL_GetTicks();
  // unsigned int frame_delta = frame_ticks - state.prev_ticks;
  // state.prev_ticks = frame_ticks;

  // EVENT LOOP
  // ---------------------------------------------------------------------------
  SDL_Event event;

  if (execute_event(event)) // Si execute_event devuelve true, se
    return true;            // quiere cerrar el juego
  // ---------------------------------------------------------------------------

  // TRY-POP DE LA RECEIVER QUEUE
  // ---------------------------------------------------------------------------
  GameState game_state = GameState();


  try {
    bool was_received = receiver_queue.pop_last_one(game_state);

    // Si no pudo recibir el game_state se queda con el ultimo game_state
    // popeado, en caso contrario utiliza el nuevo.
    if (!was_received) {
      game_state = state.last_game_state;
    } else {
      state.last_game_state = game_state;
    }

    } catch (const std::exception &e) {
      std::cerr << e.what();
  }

  // UPDATE ESTADO DEL JUEGO
  // ---------------------------------------------------------------------------
  client_sdl.world_view.update(game_state);

  /* Para revisar funcionamiento
  std::map<uint8_t, Worm> worms = game_state.get_worms();
  int worm_n = 0;
  for (auto &worm : worms) {
    worm_n++;
    std::cout << "worm_n: " << worm_n << std::endl;
    std::cout << "posx: " << worm.second.get_pos_x() << std::endl;
    std::cout << "posy: " << worm.second.get_pos_y() << std::endl;
    std::cout << "dir: " << worm.second.get_direction() << std::endl;
    std::cout << "state: " << worm.second.get_state() << std::endl;
  }
  */

  // ---------------------------------------------------------------------------

  // RENDER DE TEXTURAS
  // ---------------------------------------------------------------------------
  client_sdl.renderer.Clear();
  client_sdl.world_view.render(frame_ticks, state);
  std::string text =
      "Position: " +
      std::to_string(game_state.get_worms()[player_id].get_pos_x()) +
      ", state: " +
      (print_state(game_state.get_worms()[player_id].get_state())) +
      ", direction: " +
      std::to_string(game_state.get_worms()[player_id].get_direction());

  Font font(RESOURCES_PATH "/Vera.ttf", 12);

  Texture text_sprite(
      client_sdl.renderer,
      (font).RenderText_Blended(text, SDL_Color{255, 255, 255, 255}));

  client_sdl.renderer.Copy(
      text_sprite, NullOpt,
      Rect(0, 0, text_sprite.GetWidth(), text_sprite.GetHeight()));

  // ---------------------------------------------------------------------------

  // Show rendered frame
  client_sdl.renderer.Present();

  return false;
}


bool Client::execute_event(SDL_Event &event) {
  auto worm_client = state.last_game_state.get_worms()[player_id];
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) { // Cierra el juego
      handle_finish_game();
      return true;

    } else if (event.type == SDL_KEYDOWN) { // Aprieta una tecla
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
      case SDLK_q:
        handle_finish_game();
        return true;
      case SDLK_RIGHT:
        if (worm_client.get_state() != WORM_STATES::MOVING)
          handle_start_moving(RIGHT);
        break;
      case SDLK_LEFT:
        if (worm_client.get_state() != WORM_STATES::MOVING)
          handle_start_moving(LEFT);
        break;
      case SDLK_RETURN:
        if (worm_client.get_state() != WORM_STATES::JUMPING)
          handle_jump_forward(worm_client.get_direction());
        break;
      case SDLK_BACKSPACE:
        if (worm_client.get_state() != WORM_STATES::JUMPING)
          handle_jump_backward(worm_client.get_direction());
        break;
      case SDLK_UP:
        if (worm_client.get_state() != WORM_STATES::AIMING)
          handle_start_aiming(UP);
        break;
      case SDLK_DOWN:
        if (worm_client.get_state() != WORM_STATES::AIMING)
          handle_start_aiming(DOWN);
        break;
      case SDLK_1:
        break;
      case SDLK_i:
          client_sdl.resource_pool.turn_music_volume_down();
        break;
      case SDLK_o:
          client_sdl.resource_pool.turn_music_volume_up();
      }

    } else if (event.type == SDL_KEYUP) { // Suelta una tecla
      switch (event.key.keysym.sym) {
      case SDLK_RIGHT:
      case SDLK_LEFT:
        if (worm_client.get_state() == WORM_STATES::MOVING)
          handle_stop_moving();
        break;
      case SDLK_RETURN:
      case SDLK_BACKSPACE:
        state.is_jumping = false;
        break;
      case SDLK_UP:
      case SDLK_DOWN:
        if (worm_client.get_state() == WORM_STATES::AIMING)
          handle_stop_aiming();
        break;
      }
    }
  }
  return false;
}


void Client::handle_start_moving(int direction) {
  std::shared_ptr<StartMoving> cmd = std::make_shared<StartMoving>(direction);
  sender_queue.try_push(cmd);
  // is_running = true;
  // state.direction = direction;
}


void Client::handle_stop_moving() {
  std::shared_ptr<StopMoving> cmd = std::make_shared<StopMoving>();
  sender_queue.try_push(cmd);
  // is_running = false;
}


void Client::handle_jump_forward(uint8_t worm_dir) {
  std::shared_ptr<Jump> cmd = std::make_shared<Jump>(worm_dir);
  sender_queue.try_push(cmd);
  // state.is_running = false;
}


void Client::handle_jump_backward(uint8_t worm_dir) {
  int jump_direction = get_opposite_direction(worm_dir);
  std::shared_ptr<Jump> cmd = std::make_shared<Jump>(jump_direction);
  sender_queue.try_push(cmd);
  // state.is_running = false;
}


void Client::handle_start_aiming(int direction) {
  std::shared_ptr<StartAiming> cmd = std::make_shared<StartAiming>(direction);
  sender_queue.try_push(cmd);
}


void Client::handle_stop_aiming() {
  std::shared_ptr<StopAiming> cmd = std::make_shared<StopAiming>();
  sender_queue.try_push(cmd);
}


int Client::get_opposite_direction(uint8_t worm_dir) {
  return (worm_dir == LEFT ? RIGHT : LEFT);
}


void Client::handle_finish_game() {
  prot.close_socket();
  sender_queue.close();
  // receiver_queue.close();
}


std::string Client::print_state(uint8_t state) {
  switch (state) {
  case 0:
    return "idle";
  case 1:
    return "running";
  case 2:
    return "jumping";
  case 3:
    return "aiming";
  case 4:
    return "shooting";
  default:
    return " ";
  }
}
