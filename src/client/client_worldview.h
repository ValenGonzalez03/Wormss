#ifndef CLIENT_WORLDVIEW_H_
#define CLIENT_WORLDVIEW_H_

#include "client_beam.h"
#include "client_worm_view.h"
#include "client_resource_pool.h"
#include "../common/game_state.h"
#include "client_state.h"
#include "client_position_converter.h"
#include <vector>

class WorldView {
private:
  ResourcePool &resource_pool;
  SDL2pp::Renderer &renderer;
  std::vector<Beam> beams;
  std::list<Worm> worms;

public:
  // Crea una WorldView con una referencia a una resource pool
  // y a un renderer
  WorldView(ResourcePool &res_pool, SDL2pp::Renderer &rend)
      : resource_pool(res_pool), renderer(rend) {}

  // Convierte la posicion pasada por parámetro de m a px, 
  // crea la short_beam y la agrega al WorldView
  void add_short_beam(int pos_x, int pos_y, int angle);

  // Convierte la posicion pasada por parámetro de m a px, 
  // crea la long_beam y la agrega al WorldView
  void add_long_beam(int pos_x, int pos_y, int angle);

  // Recibe el estado de juego y actualiza la world_view
  // con los nuevos datos
  void update(GameState &game_state);

  // Renderiza la world_view
  void render(int frame, client_state &worm_state);

  //void render_text(const std::string text, std::string font_name, int pos_x, int pos_y);
};

#endif