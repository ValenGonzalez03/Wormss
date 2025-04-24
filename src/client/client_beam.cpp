#include "client_beam.h"

Beam::Beam(int width, int height, int pos_x, int pos_y, int angle,
           std::vector<SDL2pp::Texture *> &beam_texture, SDL2pp::Renderer &rend)
    : width(width), height(height), pos_x(pos_x), pos_y(pos_y), angle(angle),
      texture(beam_texture.front()), renderer(rend) {}

void Beam::render(int frame) {
  texture->SetAlphaMod(255);
  // std::cout << "Pos x viga (m): " << pos_x_px;
  // std::cout << " Pos y viga (m): " << pos_y_px << std::endl;
  renderer.Copy(*texture, SDL2pp::NullOpt, // Size
                SDL2pp::Rect(pos_x, pos_y, width, height), -angle,
                SDL2pp::NullOpt, // rotation center - not needed
                SDL_FLIP_NONE);
}
