#ifndef CLIENT_EXPLOSION_H_
#define CLIENT_EXPLOSION_H_

#include "../../common/game_state.h"
#include "../../common/game_constants.h"
#include "../client_resource_pool.h"

class Explosion {
private:
  int pos_x; // En pixeles
  int pos_y; // En pixeles
  float radius; // En pixeles
  std::vector<float> ray_fractions;
  int frame_where_started;
  bool finished = false;
  //std::vector<SDL2pp::Texture *> texture; // Vector de texturas
  SDL2pp::Renderer &renderer;

public:

  explicit Explosion();

  // Crea un Missile con un renderer y las texturas correspondientes
  explicit Explosion(int pos_x, int pos_y, int radius, std::vector<float> fractions, int start_frame, SDL2pp::Renderer &rend);

  void update(int frame);

  // Renderiza el misil pasado por parámetro según el estado del mismo
  void render(int frame);

  bool has_finished();

};

#endif