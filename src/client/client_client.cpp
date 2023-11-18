#include "client_client.h"

#include <list>

using namespace SDL2pp;

const float RATE = 1 / 60;
const float RATIO_MTS_PX = 210.0 / 9.0; // 23,3 periodico

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

  client_sdl.world_view.add_short_beam(100, 100);
  client_sdl.world_view.add_short_beam(200, 200);
  client_sdl.world_view.add_short_beam(250, 20);
  client_sdl.world_view.add_long_beam(600, 500);

  // Load sprites image as a new texture; since there's no alpha channel
  // but we need transparency, use helper surface for which set color key
  // to color index 0 -> black background on image will be transparent on our
  // texture
  // std::string image_path = "/Images/Worms/wwalk.png";
  // Texture sprites(renderer, Surface(RESOURCES_PATH + image_path)
  //			.SetColorKey(true, 200));

  // Enable alpha blending for the sprites
  client_sdl.worm_walking->SetBlendMode(SDL_BLENDMODE_BLEND);

  state.prev_ticks = SDL_GetTicks();

  // Loop del ConstantRateLoop, recibe como parametro el rate, que determina
  // cuantos frames se renderizan en un segundo
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
      }

    } else if (event.type == SDL_KEYUP) { // Suelta una tecla
      switch (event.key.keysym.sym) {
      case SDLK_RIGHT:
      case SDLK_LEFT:
        if (state.is_running)
          handle_stop_moving(state.is_running);
        break;
      }
    }
  }
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

  // TODO ESTO DEBE GENERALIZARSE PARA TODOS LOS GUSANOS
  // ---------------------------------------------------------------------------
  std::list<Worm> list = game_state.get_worms();
  // Por ahora asumo que hay un solo gusano
  Worm worm = list.front();

  Position pos_mts = Position(worm.get_pos_x(), 0);
  Position pos_px = convert_to_pixels(pos_mts);

  // Nueva coordenada X del gusano
  state.position_x = pos_px.get_position_x();
  // Coordenada Y del centro de la pantalla
  int vcenter = client_sdl.renderer.GetOutputHeight() / 2;
  if (state.is_running) {
    state.run_phase = (frame_ticks / 50) % 15;
  } else {
    state.run_phase = 0;
  }

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
  if (state.position_x > client_sdl.renderer.GetOutputWidth() - 30)
    state.position_x = client_sdl.renderer.GetOutputWidth() - 30;
  else if (state.position_x < 0)
    state.position_x = 0;

  // Clear screen
  client_sdl.renderer.Clear();

  SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL; // Sin volteo por defecto

  // Pick sprite from sprite atlas based on whether
  // player is running and run animation phase
  int src_x = 10, src_y = 10; // by default, standing sprite
  if (state.is_running) {
    // Voltear horizontalmente solo si te estás moviendo a la izquierda
    if (worm.get_direction() == LEFT) {
      flip = SDL_FLIP_NONE;
    }
    src_x = 10;
    src_y = 10 + 60 * state.run_phase;
  }

  // ---------------------------------------------------------------------------

  /*
    if (state.is_running) {
      // one of 15 run animation sprites
      src_x = 10;
      src_y = 10 + 60 * state.run_phase;
    }
  */

  // RENDER DE TEXTURAS
  // ---------------------------------------------------------------------------
  client_sdl.world_view.render(1);

  // Draw player sprite
  client_sdl.worm_walking->SetAlphaMod(255); // sprite is fully opaque
  client_sdl.renderer.Copy(
      *client_sdl.worm_walking, Rect(src_x, src_y, 40, 40), // Size
      Rect((int)state.position_x, vcenter - 40, 40, 40),    // Destination
      0.0,                                                  // don't rotate
      NullOpt, // rotation center - not needed
      flip     // horizontal flip
  );

  // client_sdl.resource_pool.add_font("Vera20", "/Vera.ttf", 20);
  // client_sdl.resource_pool.add_font("Vera12", "/Vera.ttf", 12);
  // std::shared_ptr<Font> vera20_font_ptr =
  // client_sdl.resource_pool.get_font("Vera20"); std::shared_ptr<Font>
  // vera12_font_ptr = client_sdl.resource_pool.get_font("Vera12");

  std::string text = "Position: " + std::to_string((int)state.position_x) +
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

Position Client::convert_to_pixels(Position &pos_m) {
  float pos_x_px = pos_m.get_position_x() * RATIO_MTS_PX;
  float pos_y_px = pos_m.get_position_y() * RATIO_MTS_PX;
  return Position(pos_x_px, pos_y_px);
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

void Client::handle_finish_game() {
  prot.close_socket();
  sender_queue.close();
  // receiver_queue.close();
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
