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
#include "../common/game_constants.h"

using namespace SDL2pp;

const float RATE = (float)(1.0 / 60.0);

Client::Client(ClientProtocol &&prot, uint8_t player_id)
    : prot(std::move(prot)), receiver_queue(), sender_queue(),
      receiver(this->prot, receiver_queue), sender(this->prot, sender_queue),
      client_sdl(), player_id(player_id), last_game_state() {}

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
  SDL2pp::SDL sdl(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
  
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
    
    
    // INICIALIZACION DEL MUNDO
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    bool was_closed = false;
    prot.recv_world(client_sdl.world_view, &was_closed);
    
    start_threads();
  
    int worms_amount = 0;
    // Esto claramente es una mala solucion pero por ahora sirve
    while (worms_amount == 0) {
      last_game_state = receiver_queue.pop();
      worms_amount = last_game_state.get_worms().size();
    }
    std::cout << "Cantidad worms: " << (int) last_game_state.get_worms().size() << std::endl;
    for (auto worm_data : last_game_state.get_worms()) {
      client_sdl.world_view.add_worm(worm_data.second);
    }
    // ---------------------------------------------------------------------------
    // ---------------------------------------------------------------------------
    // INICIALIZACION DEL MUNDO


  // Loop del ConstantRateLoop, recibe como parametro el rate, que determina
  // cuantos frames se renderizan en un segundo

  // Ejecuta func_to_execute en cada iteracion
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
  // ---------------------------------------------------------------------------
  SDL_Event event;

  if (execute_event(event)) // Si execute_event devuelve true, se
    return true;            // quiere cerrar el juego
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // EVENT LOOP


  // TRY-POP DE LA RECEIVER QUEUE
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  GameState game_state = GameState();

  try {
    bool was_received = receiver_queue.pop_last_one(game_state);

    // Si no pudo recibir el game_state se queda con el ultimo game_state
    // popeado, en caso contrario utiliza el nuevo.
    if (!was_received) {
      game_state = last_game_state;
    } else {
      last_game_state = game_state;
    }

    } catch (const std::exception &e) {
      std::cerr << e.what();
  }
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // TRY-POP DE LA RECEIVER QUEUE


  // UPDATE ESTADO DEL JUEGO
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  client_sdl.world_view.update(game_state);
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // UPDATE ESTADO DEL JUEGO


  // RENDER DE TEXTURAS
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  client_sdl.renderer.Clear();
  client_sdl.world_view.render(frame_ticks);
  auto player_data = game_state.get_worms()[player_id];
  std::string text =
      "Pos x: " +
      std::to_string(player_data.get_pos_x()) +
      ", Pos y: " +
      std::to_string(player_data.get_pos_y()) +
      ", state: " +
      (print_state(player_data.get_state())) +
      ", direction: " +
      (player_data.get_direction() == LEFT ? "left" : "right") + 
      ", player_id: " + 
      std::to_string(player_data.get_player_id());


  Font font(RESOURCES_PATH "/Vera.ttf", 12);

  Texture text_sprite(
      client_sdl.renderer,
      (font).RenderText_Blended(text, SDL_Color{255, 255, 255, 255}));

  client_sdl.renderer.Copy(
      text_sprite, NullOpt,
      Rect(0, 0, text_sprite.GetWidth(), text_sprite.GetHeight()));



  // Show rendered frame
  client_sdl.renderer.Present();
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // RENDER DE TEXTURAS


  return false;
}


bool Client::execute_event(SDL_Event &event) {
  auto worm_client = last_game_state.get_worms()[player_id];
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
        handle_start_moving(RIGHT);
        break;
      case SDLK_LEFT:
        handle_start_moving(LEFT);
        break;
      case SDLK_RETURN:
        if (worm_client.get_state() != JUMPING)
          handle_jump_forward(worm_client.get_direction());
        break;
      case SDLK_BACKSPACE:
        if (worm_client.get_state() != JUMPING)
          handle_jump_backward(worm_client.get_direction());
        break;
      case SDLK_UP:
        if (worm_client.get_state() != AIMING)
          handle_start_aiming(UP);
        break;
      case SDLK_DOWN:
        if (worm_client.get_state() != AIMING)
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
        handle_stop_moving(); 
        break;
      case SDLK_RETURN:
      case SDLK_BACKSPACE:
        //state.is_jumping = false;
        break;
      case SDLK_UP:
      case SDLK_DOWN:
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


std::string Client::print_state(WormState state) {
  switch (state) {
  case IDLE:
    return "idle";
  case MOVING:
    return "running";
  case JUMPING:
    return "jumping";
  case AIMING:
    return "aiming";
  case SHOOTING:
    return "shooting";
  default:
    return "unknown";
  }
}
