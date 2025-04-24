#ifndef CLIENT_WORLDVIEW_H_
#define CLIENT_WORLDVIEW_H_

#include "../common/game_state.h"
#include "client_beam.h"
#include "client_resource_pool.h"
#include "client_worm.h"
#include <vector>

class WorldView {
private:
  ResourcePool &resource_pool;
  SDL2pp::Renderer &renderer;
  std::vector<Beam> beams;
  std::map<uint8_t, Worm> worms;

  void render_background();

public:
  // Crea una WorldView con una referencia a una resource pool
  // y a un renderer
  WorldView(ResourcePool &res_pool, SDL2pp::Renderer &rend);

  // void add_worms(std::vector<std::vector<float>> spawn_points);

  void add_worm(WormData data);
  // Convierte la posicion pasada por parámetro de m a px,
  // crea la long_beam y la agrega al WorldView
  void add_beam(float pos_x, float pos_y, float width, float height, int angle);

  // Recibe el estado de juego y actualiza la world_view
  // con los nuevos datos
  void update(GameState &game_state);

  // Renderiza la world_view
  void render(int frame);

  // void render_text(const std::string text, std::string font_name, int pos_x,
  // int pos_y);

  // Setea un background al mundo
  void set_background(const std::string &path);
};

#endif