#include "client_client.h"

using namespace SDL2pp;

Client::Client(Socket &&skt)
    : prot(std::move(skt)), receiver_queue(), sender_queue(),
      receiver(prot, receiver_queue), sender(prot, sender_queue) {}

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

  // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
  Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                1280, 720, SDL_WINDOW_RESIZABLE);

  // Create accelerated video renderer with default driver
  Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Create resource pool
  ResourcePool resource_pool(renderer);

  WorldView world_view(resource_pool, renderer);
  world_view.add_short_beam(100, 100);
  world_view.add_short_beam(200, 200);
  world_view.add_short_beam(250, 20);
  world_view.add_long_beam(600, 500);

  // Load sprites image as a new texture; since there's no alpha channel
  // but we need transparency, use helper surface for which set color key
  // to color index 0 -> black background on image will be transparent on our
  // texture
  // std::string image_path = "/Images/Worms/wwalk.png";
  // Texture sprites(renderer, Surface(RESOURCES_PATH + image_path)
  //			.SetColorKey(true, 200));

  Texture *worm_walking = resource_pool.get_worm_walking();

  // Enable alpha blending for the sprites
  worm_walking->SetBlendMode(SDL_BLENDMODE_BLEND);

  // Game state
  bool is_running = false; // whether the character is currently running
  int run_phase = -1;      // run animation phase
  float position = 0.0;    // player position

  unsigned int prev_ticks = SDL_GetTicks();
  // Main loop
  while (1) {
    // Timing: calculate difference between this and previous frame
    // in milliseconds
    unsigned int frame_ticks = SDL_GetTicks();
    unsigned int frame_delta = frame_ticks - prev_ticks;
    prev_ticks = frame_ticks;

    // Event processing:
    // - If window is closed, or Q or Escape buttons are pressed,
    //   quit the application
    // - If Right key is pressed, character would run
    // - If Right key is released, character would stop
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        return 0;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
        case SDLK_q:
          return 0;
        case SDLK_RIGHT:
          is_running = true;
          break;
        }
      } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
        case SDLK_RIGHT:
          is_running = false;
          break;
        }
      }
    }

    // Update game state for this frame:
    // if character is runnung, move it to the right
    if (is_running) {
      position += frame_delta * 0.2;
      run_phase = (frame_ticks / 100) % 8;
    } else {
      run_phase = 0;
    }

    // If player passes past the right side of the window, wrap him
    // to the left side
    if (position > renderer.GetOutputWidth())
      position = -50;

    int vcenter =
        renderer.GetOutputHeight() / 2; // Y coordinate of window center

    // Clear screen
    renderer.Clear();

    // Pick sprite from sprite atlas based on whether
    // player is running and run animation phase
    int src_x = 10, src_y = 10; // by default, standing sprite
    if (is_running) {
      // one of 8 run animation sprites
      src_x = 10;
      src_y = 10 + 60 * run_phase;
    }

    world_view.render(1);

    // Draw player sprite
    worm_walking->SetAlphaMod(255); // sprite is fully opaque
    renderer.Copy(*worm_walking, Rect(src_x, src_y, 40, 40), // Size
                  Rect((int)position, vcenter - 40, 40, 40), // Destination
                  0.0,                                       // don't rotate
                  NullOpt,            // rotation center - not needed
                  SDL_FLIP_HORIZONTAL // vertical flip
    );

    // Show rendered frame
    renderer.Present();

    // Frame limiter: sleep for a little bit to not eat 100% of CPU
    SDL_Delay(1);
  }

  // Here all resources are automatically released and libraries deinitialized
  return 0;
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
