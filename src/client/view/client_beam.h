#ifndef CLIENT_BEAM_H_
#define CLIENT_BEAM_H_

#include "../../common/position.h"
#include <SDL2pp/SDL2pp.hh>

class Beam {
 private:
  // Position position;
  int width;
  int height;
  int pos_x;
  int pos_y;
  float angle;
  SDL2pp::Texture *texture;
  SDL2pp::Renderer &renderer;
  SDL_Point pivot = {0, 0};

 public:
  // Crea una Beam en la posicion en px pasada por parámetro
  Beam(int width, int height, int pos_x, int pos_y, float angle, std::vector<SDL2pp::Texture *> &beam_texture,
       SDL2pp::Renderer &rend);

  // Renderiza la Beam en su posición
  void render(int frame, int camera_x, int camera_y);
};

#endif