#include "client_client.h"

#include <list>

#include "../common/create_game.h"
#include "../common/join_game.h"
#include "../common/start_game.h"
#include "../common/start_moving.h"
#include "../common/stop_moving.h"
#include "../common/jump.h"

using namespace SDL2pp;

const float RATE = 1 / 60;
// const float RATIO_MTS_PX = 210.0 / 9.0; // 23,3 periodico

Client::Client(Socket &&skt)
    : prot(std::move(skt)), receiver_queue(), sender_queue(),
      receiver(prot, receiver_queue), sender(prot, sender_queue), state(),
      client_sdl() {}

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
  SDL sdl(SDL_INIT_VIDEO);

  // Initialize SDL_ttf library
  SDLTTF ttf;

  // client_sdl.world_view.add_short_beam(100, 100);
  // client_sdl.world_view.add_short_beam(200, 200);
  // client_sdl.world_view.add_short_beam(250, 20);
  client_sdl.world_view.add_long_beam(0, 1, 0);
  client_sdl.world_view.add_long_beam(6, 1, 30);
  // client_sdl.world_view.add_long_beam(3, 3);

  // Enable alpha blending for the sprites
  // client_sdl.worm_walking->SetBlendMode(SDL_BLENDMODE_BLEND);

  state.prev_ticks = SDL_GetTicks();

  // LOBBY DE UNA PARTIDA
  // ---------------------------------------------------------------------------

  // Por ahora una pequeña interaccion mediante la terminal para que el cliente
  // decida si crear una partida o unirse a una, si se quiere unir debe ingresar
  // el codigo del juego.
  char option_selected = '\0';
  do {
    std::cout << "Ingrese 'c' si quiere crear una partida o 'j' si quiere "
                 "unirse a una:"
              << std::endl;
    std::cin >> option_selected;
  } while (option_selected != 'c' && option_selected != 'j');

  if (option_selected == 'c') {
    std::cout << "Juego creado, esperando jugadores..." << std::endl;
    CreateGame create_comm = CreateGame();
    prot.send_command(create_comm);
  } else if (option_selected == 'j') {
    std::cout << "Ingrese el codigo de la partida para unirse:" << std::endl;
    int game_id;
    std::cin >> game_id;
    JoinGame join_comm(game_id);
    prot.send_command(join_comm);
  }

  // Recibe el player_id
  int player_id = prot.receive_id();
  std::cout << "Tu player_id es: " << player_id << std::endl;
  // Si se crea una partida tambien se recibe el game_id
  int game_id;
  if (option_selected == 'c') {
    std::cout << "b1" << std::endl;
    game_id = prot.receive_id();
    std::cout << "El game id es: " << game_id << std::endl;
  }

  char command_lobby = '\0';
  while (command_lobby != 's') {
    std::cout
        << "El creador de la partida cuando quiere empezarla debe ingresar 's'"
        << std::endl;
    std::cin >> command_lobby;
  }
  StartGame start = StartGame(game_id);
  prot.send_command(start);

  // ---------------------------------------------------------------------------

  // Loop del ConstantRateLoop, recibe como parametro el rate, que determina
  // cuantos frames se renderizan en un segundo

  start_threads(); //Inicializo los threads sender y receiver

  loop(std::chrono::duration<float>((float)RATE));

  return 0;
}

bool Client::func_to_execute() {
  // Timing: calculate difference between this and previous frame
  // in milliseconds
  unsigned int frame_ticks = SDL_GetTicks();
  unsigned int frame_delta = frame_ticks - state.prev_ticks;
  state.prev_ticks = frame_ticks;

  // EVENT LOOP
  // ---------------------------------------------------------------------------
  SDL_Event event;

  if (execute_event(event)) // Si execute_event devuelve true, se
    return true;            // quiere cerrar el juego
  // ---------------------------------------------------------------------------

  // TRY-POP DE LA RECEIVER QUEUE
  // ---------------------------------------------------------------------------
  GameState game_state = GameState();

  bool was_received = receiver_queue.pop_last_one(game_state);

  if (!was_received) {
    game_state = state.last_game_state;
  } else {
    state.last_game_state = game_state;
  }

  // ---------------------------------------------------------------------------

  // RENDER DE TEXTURAS
  // ---------------------------------------------------------------------------

  // PRUEBA RENDERIZADO MULTIPLES WORMS
  // state.last_game_state.add_worm(1.3, 1.5, 1, 0);
  // state.last_game_state.add_worm(5.2, 5.8, 0, 0);
  client_sdl.world_view.update(state.last_game_state);

  std::list<Worm> worms = state.last_game_state.get_worms();
  int worm_n = 0;
  for (auto &worm : worms) {
    worm_n++;
    std::cout << "worm_n: " << worm_n << std::endl;
    std::cout << "posx: " << worm.get_pos_x() << std::endl;
    std::cout << "posy: " << worm.get_pos_y() << std::endl;
    std::cout << "dir: " << worm.get_direction() << std::endl;
    std::cout << "state: " << worm.get_state() << std::endl;
  }

  client_sdl.renderer.Clear();
  client_sdl.world_view.render(frame_ticks, state);

  
  std::string text =
      "Position: " +
      std::to_string(game_state.get_worms().front().get_pos_x()) +
      ", running: " + (state.is_running ? "true" : "false") +
      ", direction: " + std::to_string(int(state.direction));

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

  // Frame limiter: sleep for a little bit to not eat 100% of CPU
  SDL_Delay(1);

  return false;
}

void Client::handle_start_moving(int direction, bool &is_running) {
  std::shared_ptr<StartMoving> cmd = std::make_shared<StartMoving>(direction);
  sender_queue.try_push(cmd);
  is_running = true;
  state.direction = direction;
}

void Client::handle_stop_moving(bool &is_running) {
  std::shared_ptr<StopMoving> cmd = std::make_shared<StopMoving>();
  sender_queue.try_push(cmd);
  is_running = false;
}

void Client::handle_jump_forward(bool &is_jumping) {
  std::shared_ptr<Jump> cmd = std::make_shared<Jump>(state.direction);
  sender_queue.try_push(cmd);
  is_jumping = true;
}

void Client::handle_jump_backward(bool &is_jumping) {
  int jump_direction = get_opposite_direction();
  std::shared_ptr<Jump> cmd = std::make_shared<Jump>(jump_direction);
  sender_queue.try_push(cmd);
  is_jumping = true;
}

int Client::get_opposite_direction() {
  if (state.direction == LEFT)
    return RIGHT;
  else // if (state.direction == RIGHT)
    return LEFT;
}

void Client::handle_finish_game() {
  prot.close_socket();
  sender_queue.close();
  // receiver_queue.close();
}


bool Client::execute_event(SDL_Event &event) {
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
          if (!state.is_running)
            handle_start_moving(RIGHT, state.is_running);
          break;
        case SDLK_LEFT:
          if (!state.is_running)
            handle_start_moving(LEFT, state.is_running);
          break;
        case SDLK_RETURN:
          if (!state.is_jumping)
            handle_jump_forward(state.is_jumping);
          break;
        case SDLK_BACKSPACE:
          if (!state.is_jumping)
            handle_jump_backward(state.is_jumping);
          break;
      }

    } else if (event.type == SDL_KEYUP) { // Suelta una tecla
      switch (event.key.keysym.sym) {
        case SDLK_RIGHT:
        case SDLK_LEFT:
          if (state.is_running)
            handle_stop_moving(state.is_running);
          break;
        case SDLK_RETURN:
        case SDLK_BACKSPACE:
          state.is_jumping = false;
          break;
      }
    }
  }
  return false;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

// Gradient texture to be generated
/*
        Texture gradient(renderer, SDL_PIXELFORMAT_BGR888,
   SDL_TEXTUREACCESS_STATIC, 1, 256);
        {
                // Array holding RGB values for 256 pixels
                unsigned char grad[256 * 3];
                int n = 0; // counter

                // Fill array with fading gray values from white to black; its
   contents will be
                // 255,255,255, 254,254,254, 253,253,253 ... 1,1,1, 0,0,0
                std::generate(grad, grad + sizeof(grad), [&](){ return 255 -
   n++ / 3; });

                // Update texture with our raw color data, enable blending and
   set color
                // and alpha modulation, so when rendered our texture will be
   dark cyan gradient.Update(NullOpt, grad, 3)
                        .SetBlendMode(SDL_BLENDMODE_BLEND)
                        .SetColorMod(0, 255, 255)
                        .SetAlphaMod(85);
        }

        // Load font, 12pt size
        Font font(DATA_PATH "/Vera.ttf", 12);
*/

// Copy our gradient texture, stretching it to the whole window
//		renderer.Copy(
//				gradient,
//				NullOpt,
//				Rect(0, vcenter, renderer.GetOutputWidth(),
// renderer.GetOutputHeight() / 2)
//			);

/*
                // Draw the same sprite, below the first one, 50%
   transparent and
                // vertically flipped. It'll look like reflection in the
   mirror sprites.SetAlphaMod(127); // 50% transparent renderer.Copy(
                                sprites,
                                Rect(src_x, src_y, 50, 50),
                                Rect((int)position, vcenter, 50, 50),
                                0.0,              // don't rotate
                                NullOpt,          // rotation center - not
   needed SDL_FLIP_VERTICAL // vertical flip
                        );

                // Create text string to render
                std::string text =
                        "Position: "
                        + std::to_string((int)position)
                        + ", running: "
                        + (is_running ? "true" : "false");

                // Render the text into new texture. Note that SDL_ttf
   render
                // text into Surface, which is converted into texture on the
   fly Texture text_sprite( renderer, font.RenderText_Blended(text,
   SDL_Color{255, 255, 255, 255})
                        );

                // Copy texture into top-left corner of the window
                renderer.Copy(text_sprite, NullOpt, Rect(0, 0,
   text_sprite.GetWidth(), text_sprite.GetHeight()));
*/



   // // Update game state for this frame:
  // // if character is runnung, move it to the right
  // if (state.is_running) {
  //   if (state.direction == RIGHT) {
  //   state.position_x += frame_delta * 0.2;
  //   state.run_phase =
  //       (frame_ticks / 50) % 15; // Algunos retoques en run_phase para que se
  //                                // vea mas fluido y con todos los frames
  //   }
  //   else if (state.direction == LEFT) {
  //     state.position_x += frame_delta * 0.2 * -1;
  //     state.run_phase =
  //       (frame_ticks / 50) % 15;
  //   }
  // } else {
  //   state.run_phase = 0;
  // }

  // If player passes past the right side of the window, wrap him
  // to the left side
  // if (pos_x_px > client_sdl.renderer.GetOutputWidth() - 30)
  //  pos_x_px = client_sdl.renderer.GetOutputWidth() - 30;
  // else if (pos_x_px < 0)
  //  pos_x_px = 0;

  // Clear screen
  // client_sdl.renderer.Clear();

  // SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL; // Sin volteo por defecto

  // Pick sprite from sprite atlas based on whether
  // player is running and run animation phase
  // int src_x = 10, src_y = 10; // by default, standing sprite
  // if (state.is_running) {
  // Voltear horizontalmente solo si te estás moviendo a la izquierda
  //  if (worm.get_direction() == LEFT) {
  //    flip = SDL_FLIP_NONE;
  //  }
  //  src_x = 10;
  //  src_y = 10 + 60 * state.run_phase;
  //}

  /*
    if (state.is_running) {
      // one of 15 run animation sprites
      src_x = 10;
      src_y = 10 + 60 * state.run_phase;
    }
  */

  // client_sdl.world_view.render(1, state);

  // Draw player sprite
  // client_sdl.worm_walking->SetAlphaMod(255); // sprite is fully opaque
  // client_sdl.renderer.Copy(
  //    *client_sdl.worm_walking, Rect(src_x, src_y, 40, 40), // Size
  //    Rect((int)pos_x_px, vcenter - 40, 40, 40),    // Destination
  //    0.0,                                                  // don't rotate
  //    NullOpt, // rotation center - not needed
  //    flip     // horizontal flip
  //);

  // client_sdl.resource_pool.add_font("Vera20", "/Vera.ttf", 20);
  // client_sdl.resource_pool.add_font("Vera12", "/Vera.ttf", 12);
  // std::shared_ptr<Font> vera20_font_ptr =
  // client_sdl.resource_pool.get_font("Vera20"); std::shared_ptr<Font>
  // vera12_font_ptr = client_sdl.resource_pool.get_font("Vera12");

