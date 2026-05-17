#ifndef CLIENT_WORLDVIEW_H_
#define CLIENT_WORLDVIEW_H_

#include "../../common/game_state.h"
#include "client_beam.h"
#include "../client_resource_pool.h"
#include "client_camera.h"
#include "client_worm.h"
#include "client_explodable.h"
#include "client_explosion.h"
#include <utility>
#include <string>
#include <vector>
#include <map>

class WorldView {
 private:
  ResourcePool &resource_pool;
  SDL2pp::Renderer &renderer;
  Camera &camera;
  std::vector<Beam> beams;
  std::map<uint8_t, Worm> worms;
  std::map<uint8_t, Explodable> explodables;
  std::vector<Explosion> explosions;
  const uint8_t &player_id;

  void render_background();

  std::pair<float, float> get_explodable_size(BODY_TYPES type);

  // Muestra el estado del gusano
  std::string print_state(WormState state);

  // Muestra el arma seleccionada del gusano
  std::string print_weapon_selected(WeaponType weapon);

 public:
  // Crea una WorldView con una referencia a una resource pool y a un renderer
  WorldView(ResourcePool &res_pool, SDL2pp::Renderer &rend,  // NOLINT(runtime/references)
            Camera &camera,                                  // NOLINT(runtime/references)
            const uint8_t &player_id);
  // void add_worms(std::vector<std::vector<float>> spawn_points);

  void add_worm(WormData data);

  // Convierte la posicion pasada por parámetro de m a px, crea la long_beam y la agrega al WorldView
  void add_beam(float pos_x, float pos_y, float width, float height, float angle);

  Explodable add_explodable(ExplodableData data);

  void add_explosion(ExplosionData data, int frame);

  // Recibe el estado de juego y actualiza la world_view con los nuevos datos
  void update(const GameState &game_state, int frame);

  // Renderiza la world_view
  void render(int frame);

  void render_text(const WormData &worm_data);

  // Setea un background al mundo
  void set_background(const std::string &path);
};

#endif
