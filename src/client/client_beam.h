#ifndef CLIENT_BEAM_H_
#define CLIENT_BEAM_H_

#include "../common/position.h"
#include <SDL2pp/SDL2pp.hh>

class Beam {
private:
  Position position;
  SDL2pp::Texture *texture;
  SDL2pp::Renderer &renderer;
  int angle;
  SDL_Point pivot = {0, 0};

public:
  // Crea una Beam en la posicion en px pasada por parámetro
  Beam(Position &pos, std::vector<SDL2pp::Texture *> &beam_texture,
       SDL2pp::Renderer &rend, int angle);

  // Renderiza la Beam en su posición
  void render(int frame);
  
};

#endif