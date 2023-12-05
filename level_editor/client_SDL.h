#ifndef CLIENT_SDL_H_
#define CLIENT_SDL_H_

#include <SDL2pp/SDL2pp.hh>


// Contiene los datos del cliente SDL, como la window, el renderer
struct client_SDL {
  // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
  SDL2pp::Window window =
      SDL2pp::Window("Level editor", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_RESIZABLE);
  // Create accelerated video renderer with default driver
  SDL2pp::Renderer renderer =
      SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED);
};

#endif