#ifndef CLIENT_SDL_H_
#define CLIENT_SDL_H_

#include "client_worldview.h"

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

#endif