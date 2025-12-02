#ifndef CLIENT_WORLDVIEW_H_
#define CLIENT_WORLDVIEW_H_

#include "../common/game_state.h"
#include "client_beam.h"
#include "client_resource_pool.h"
#include "client_worm.h"
#include "client_missile.h"
#include "client_explosion.h"
#include <vector>

class WorldView {
private:
  ResourcePool &resource_pool;
  SDL2pp::Renderer &renderer;
  std::vector<Beam> beams;
  std::map<uint8_t, Worm> worms;
  std::map<uint8_t, Missile> missiles;
  std::vector<Explosion> explosions;

  void render_background();

  // Muestra el estado del gusano
  std::string print_state(WormState state);
  
  // Muestra el arma seleccionada del gusano
  std::string print_weapon_selected(WeaponType weapon);
  
  public:
  // Crea una WorldView con una referencia a una resource pool
  // y a un renderer
  WorldView(ResourcePool &res_pool, SDL2pp::Renderer &rend);
  
  // void add_worms(std::vector<std::vector<float>> spawn_points);
  
  void add_worm(WormData data);
  
  // Convierte la posicion pasada por parámetro de m a px,
  // crea la long_beam y la agrega al WorldView
  void add_beam(float pos_x, float pos_y, float width, float height, int angle);
  
  Missile add_missile(MissileData data);
  
  void add_explosion(ExplosionData data, int frame);
  
  // Recibe el estado de juego y actualiza la world_view
  // con los nuevos datos
  void update(GameState &game_state, int frame);
  
  // Renderiza la world_view
  void render(int frame);
  
  void render_text(WormData &worm_data);

  // Setea un background al mundo
  void set_background(const std::string &path);
};

#endif