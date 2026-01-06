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
#include "../common/commands/start_shooting.h"
#include "../common/commands/stop_game.h"
#include "../common/commands/change_weapon.h"
#include "../common/game_constants.h"

using namespace SDL2pp;

const float RATE = (float)(1.0 / 60.0);

Client::Client(ClientProtocol &&prot, uint8_t player_id)
    : prot(std::move(prot)), receiver_queue(), sender_queue(),
      receiver(this->prot, receiver_queue, keep_playing), sender(this->prot, sender_queue, keep_playing),
      client_sdl(), player_id(player_id), last_game_state() {}

void Client::start_threads()
{
  sender.start();
  receiver.start();
}

void Client::join_threads()
{
  sender.join();
  receiver.join();
}

int Client::run()
{

  // Initialize SDL library
  SDL2pp::SDL sdl(SDL_INIT_AUDIO | SDL_INIT_VIDEO);

  // Initialize SDL_ttf library
  SDLTTF ttf;

  // Initialize SDLMIXER library
  SDLMixer mixer;
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
  {
    throw std::runtime_error("SDL_mixer could not initialize!");
  }

  // INICIALIZO LA RESOURCE POOL (LAS TEXTURAS, MUSICA, ETC)
  client_sdl.resource_pool.initialize();
  client_sdl.resource_pool.play_music();

  // INICIALIZACION DEL MUNDO
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  recv_world();

  start_threads();

  int worms_amount = 0;
  // Esto claramente es una mala solucion pero por ahora sirve
  std::cout << "LLEGA ACA EN CLIENT" << std::endl;
  last_game_state = receiver_queue.pop();
  std::cout << "Cantidad worms: " << (int)last_game_state.get_worms().size() << std::endl;
  for (auto worm_data : last_game_state.get_worms())
  {
    client_sdl.world_view.add_worm(worm_data.second);
  }
  prot.send_client_ready();
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // INICIALIZACION DEL MUNDO

  // Loop del ConstantRateLoop, recibe como parametro el rate, que determina
  // cuantos frames se renderizan en un segundo

  // Ejecuta func_to_execute en cada iteracion
  loop(std::chrono::duration<float>((float)RATE));

  return 0;
}

void Client::recv_world()
{
  bool was_closed = false;

  std::string world_name = prot.recv_string(&was_closed); // Por ahora no se hace nada con el nombre
  std::string background_name = prot.recv_string(&was_closed);
  client_sdl.world_view.set_background(background_name);

  uint8_t beams_number = prot.recv_byte(&was_closed);
  // int beams_number = prot.recv_beams_number(&was_closed);

  std::cout << "Nombre mundo: " << world_name << ", Cantidad vigas: " << (int)beams_number << std::endl;
  for (int i = 0; i < beams_number; i++)
  {
    BeamAttr beam_attr = prot.recv_beam(&was_closed);

    if (beam_attr.width == 6.0f || beam_attr.width == 3.0f)
    {
      client_sdl.world_view.add_beam(beam_attr.pos_x, beam_attr.pos_y, beam_attr.width, BEAM_HEIGHT, beam_attr.angle);
    }
    else
    {
      std::cout << "Error tamanio viga" << std::endl;
    }
  }
}

bool Client::func_to_execute()
{
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

  try
  {
    bool was_received = receiver_queue.pop_last_one(game_state);

    // Si no pudo recibir el game_state se queda con el ultimo game_state
    // popeado, en caso contrario utiliza el nuevo.
    if (!was_received)
    {
      game_state = last_game_state;
    }
    else
    {
      last_game_state = game_state;
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what();
  }

  if (game_state.is_game_finished())
  {
    std::cout << "El juego ha terminado" << std::endl;
    prot.close_socket();
    sender_queue.close();
    return true; // Se cierra el juego
  }
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // TRY-POP DE LA RECEIVER QUEUE

  // UPDATE ESTADO DEL JUEGO
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  client_sdl.world_view.update(game_state, frame_ticks);
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // UPDATE ESTADO DEL JUEGO

  // RENDER DE TEXTURAS
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  client_sdl.renderer.Clear();
  client_sdl.world_view.render(frame_ticks);
  client_sdl.world_view.render_text(game_state.get_worms()[player_id]);
        
  // Show rendered frame
  client_sdl.renderer.Present();
  // ---------------------------------------------------------------------------
  // ---------------------------------------------------------------------------
  // RENDER DE TEXTURAS

  return false;
}

bool Client::execute_event(SDL_Event &event)
{
  auto worm_client = last_game_state.get_worms()[player_id];
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    { // Cierra el juego
      handle_quit_game();
      return true;
    }
    else if (event.type == SDL_KEYDOWN)
    { // Aprieta una tecla
      int key_mov_dir;
      int key_aim_dir;
      switch (event.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        handle_stop_game();
        break;
      case SDLK_q:
        handle_quit_game();
        return true;
        break;
      case SDLK_RIGHT:
      case SDLK_LEFT:
        if (mov_keys_pressed.count(event.key.keysym.sym) == 0)
        {
          mov_keys_pressed.insert(event.key.keysym.sym);
        }
        key_mov_dir = (event.key.keysym.sym == SDLK_RIGHT ? RIGHT : LEFT);
        handle_start_moving(key_mov_dir);
        break;
      case SDLK_RETURN:
        if (worm_client.get_state() != JUMPING)
          handle_jump_forward(worm_client.get_direction(), JUMP_FORWARD);
        break;
      case SDLK_BACKSPACE:
        if (worm_client.get_state() != JUMPING)
          handle_jump_backward(worm_client.get_direction(), JUMP_BACKWARD);
        break;
      case SDLK_UP:
      case SDLK_DOWN:
        if (aim_keys_pressed.count(event.key.keysym.sym) == 0)
        {
          aim_keys_pressed.insert(event.key.keysym.sym);
        }
        key_aim_dir = (event.key.keysym.sym == SDLK_UP ? UP : DOWN);
        handle_start_aiming(key_aim_dir);
        break;
      case SDLK_SPACE:
        handle_start_shooting();
        break;
      case SDLK_1:
        handle_change_weapon(BAZOOKA);
        break;
      case SDLK_2:
        handle_change_weapon(BAT);
        break;
      case SDLK_i:
        client_sdl.resource_pool.turn_music_volume_down();
        break;
      case SDLK_o:
        client_sdl.resource_pool.turn_music_volume_up();
        break;
      }
    }
    else if (event.type == SDL_KEYUP)
    { // Suelta una tecla
      switch (event.key.keysym.sym)
      {
      case SDLK_RIGHT:
      case SDLK_LEFT:
        mov_keys_pressed.erase(event.key.keysym.sym);
        if (mov_keys_pressed.empty())
        {
          handle_stop_moving();
        }
        break;
      case SDLK_RETURN:
      case SDLK_BACKSPACE:
        // state.is_jumping = false;
        break;
      case SDLK_UP:
      case SDLK_DOWN:
        aim_keys_pressed.erase(event.key.keysym.sym);
        if (aim_keys_pressed.empty())
        {
          handle_stop_aiming();
        }
        break;
      }
    }
  }
  return false;
}

void Client::handle_start_moving(int direction)
{
  std::shared_ptr<StartMoving> cmd = std::make_shared<StartMoving>(player_id, direction);
  sender_queue.try_push(cmd);
  // is_running = true;
  // state.direction = direction;
}

void Client::handle_stop_moving()
{
  std::shared_ptr<StopMoving> cmd = std::make_shared<StopMoving>(player_id);
  sender_queue.try_push(cmd);
  // is_running = false;
}

void Client::handle_jump_forward(uint8_t worm_dir, uint8_t jump_type)
{
  std::shared_ptr<Jump> cmd = std::make_shared<Jump>(player_id, worm_dir, jump_type);
  sender_queue.try_push(cmd);
  // state.is_running = false;
}

void Client::handle_jump_backward(uint8_t worm_dir, uint8_t jump_type)
{
  int jump_direction = (worm_dir == LEFT ? RIGHT : LEFT); // Calculo la direccion opuesta
  std::shared_ptr<Jump> cmd = std::make_shared<Jump>(player_id, jump_direction, jump_type);
  sender_queue.try_push(cmd);
  // state.is_running = false;
}

void Client::handle_start_aiming(int direction)
{
  std::shared_ptr<StartAiming> cmd = std::make_shared<StartAiming>(player_id, direction);
  sender_queue.try_push(cmd);
}

void Client::handle_stop_aiming()
{
  std::shared_ptr<StopAiming> cmd = std::make_shared<StopAiming>(player_id);
  sender_queue.try_push(cmd);
}

void Client::handle_start_shooting()
{
  std::shared_ptr<StartShooting> cmd = std::make_shared<StartShooting>(player_id, 8);
  sender_queue.try_push(cmd);
}

void Client::handle_change_weapon(uint8_t weapon_type)
{
  std::shared_ptr<ChangeWeapon> cmd = std::make_shared<ChangeWeapon>(player_id, weapon_type);
  sender_queue.try_push(cmd);
}

void Client::handle_stop_game()
{
  std::shared_ptr<StopGame> cmd = std::make_shared<StopGame>(player_id);
  sender_queue.try_push(cmd);
}

void Client::handle_quit_game()
{
  keep_playing = false;
  sender_queue.close();
  receiver_queue.close();
  prot.close_socket();
}

std::string Client::print_weapon_selected(WeaponType weapon)
{
  switch (weapon)
  {
  case BAZOOKA:
    return "bazooka";
  case BAT:
    return "baseball bat";
  default:
    return "unknown";
  }
}
