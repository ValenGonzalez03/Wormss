#ifndef CLIENT_BEAM_H_
#define CLIENT_BEAM_H_

#include "../common/position.h"
#include <SDL2pp/SDL2pp.hh>

class Beam {
private:
  Position position;
  SDL2pp::Texture &texture;
  SDL2pp::Renderer &renderer;

public:
  // Crea una Beam en la posicion en px pasada por parámetro
  Beam(Position &pos, SDL2pp::Texture &beam_texture, SDL2pp::Renderer &rend)
      : position(pos), texture(beam_texture), renderer(rend) {}

  // Renderiza la Beam en su posición
  void render(int frame) {
    texture.SetAlphaMod(255);
    int sbeam_height = texture.GetHeight();
    int sbeam_width = texture.GetWidth();
    renderer.Copy(texture,
                  SDL2pp::Rect(0, 0, sbeam_width - 10, sbeam_height), // Size
                  SDL2pp::Rect(position.get_position_x(),
                               position.get_position_y(), sbeam_width,
                               sbeam_height)); // Destination
  }
};

#endif