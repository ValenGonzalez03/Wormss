#ifndef CLIENT_MISSILE_H_
#define CLIENT_MISSILE_H_

#include "../common/game_state.h"
#include "../common/game_constants.h"
#include "client_resource_pool.h"

class Missile {
private:
  int pos_x; // En pixeles
  int pos_y; // En pixeles
  int width; // En pixeles
  int height; // En pixeles
  float angle;
  uint8_t direction;
  uint8_t id;
  std::vector<SDL2pp::Texture *> texture; // Vector de texturas
  SDL2pp::Renderer &renderer;

  void DrawRotatedRect(SDL2pp::Renderer& renderer, int width, int height, int pos_x, int pos_y, float angle);

public:

  explicit Missile();

  // Crea un Missile con un renderer y las texturas correspondientes
  explicit Missile(int pos_x, int pos_y, int width, int heigth, float angle, uint8_t dir, uint8_t id,
    std::vector<SDL2pp::Texture *> &&textures, SDL2pp::Renderer &rend);

  void update(MissileData data);

  // Renderiza el misil pasado por parámetro según el estado del mismo
  void render(int frame);

  int get_pos_x();

  int get_pos_y();

  uint8_t get_id();
};

#endif