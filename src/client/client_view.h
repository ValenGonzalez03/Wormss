#ifndef CLIENT_VIEW_H_
#define CLIENT_VIEW_H_

#include "view/client_camera.h"
#include "view/client_worldview.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Contiene los datos del cliente SDL, como la window, el renderer
// la resource pool, el world_view, etc
struct ClientView {
  explicit ClientView(const uint8_t& player_id) :
      resource_pool(renderer), world_view(resource_pool, renderer, camera, player_id) {}

  // Create main window: 640x480 dimensions centered, resizable, "Worms" title
  SDL2pp::Window window = SDL2pp::Window("Worms", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                         SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

  // Create accelerated video renderer with default driver
  SDL2pp::Renderer renderer = SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED);

  // Creo Resource Pool para obtener las diferentes texturas de mi juego
  ResourcePool resource_pool = ResourcePool(renderer);

  // Creo la Camara para mover por el escenario
  Camera camera = Camera(SCREEN_WIDTH, SCREEN_HEIGHT);

  // Creo el WorldView encargado de manejar el renderizado completo de la vista del juego
  WorldView world_view;
};

#endif
