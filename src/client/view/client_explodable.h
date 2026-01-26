#ifndef CLIENT_EXPLODABLE_H_
#define CLIENT_EXPLODABLE_H_

#include "../../common/game_state.h"
#include "../../common/game_constants.h"
#include "../client_resource_pool.h"

class Explodable {
 private:
  int pos_x;   // En pixeles
  int pos_y;   // En pixeles
  int width;   // En pixeles
  int height;  // En pixeles
  float angle;
  uint8_t direction;
  uint8_t id;
  SDL2pp::Renderer& renderer;
  SDL2pp::Texture* texture;  // Textura

  void DrawRotatedRect(SDL2pp::Renderer& renderer, int width, int height, int pos_x, int pos_y, float angle);

 public:

  // Crea un Explodable con un renderer y las texturas correspondientes
  explicit Explodable(int pos_x, int pos_y, int width, int heigth, float angle, uint8_t dir, uint8_t id,
                      SDL2pp::Texture* texture, SDL2pp::Renderer& rend);

  void update(ExplodableData data);

  // Renderiza el Explodable pasado por parámetro según el estado del mismo
  virtual void render(int frame, int camera_x, int camera_y);

  int get_pos_x();

  int get_pos_y();

  uint8_t get_id();
};

#endif