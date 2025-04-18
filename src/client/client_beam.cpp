#include "client_beam.h"
#include "client_position_converter.h"

Beam::Beam(Position &pos, std::vector<SDL2pp::Texture *> &beam_texture,
    SDL2pp::Renderer &rend, int angle)
    : position(pos), texture(beam_texture.front()), renderer(rend),
    angle(angle) {}

void Beam::render (int frame) {
  PositionConverter converter;
  texture->SetAlphaMod(255);
  int sbeam_height = texture->GetHeight();
  int sbeam_width = texture->GetWidth();
  //std::cout << "Largo de viga (px): " << sbeam_width << std::endl;
//   std::cout << "Alto de viga (px): " << sbeam_height << std::endl;
  // float pos_x_px = position.get_position_x();
  // float pos_y_px = position.get_position_y();
  // std::cout << "Pos x viga (m): " << pos_x_px;
  // std::cout << " Pos y viga (m): " << pos_y_px << std::endl;
  int pos_x = (int) position.get_position_x() - sbeam_width / 2;
  int pos_y = (int) position.get_position_y() - sbeam_height / 2;
  renderer.Copy(
      *texture, SDL2pp::NullOpt, // Size
      SDL2pp::Rect(pos_x, pos_y , sbeam_width, sbeam_height),
                    -angle,
                    SDL2pp::NullOpt, // rotation center - not needed
                    SDL_FLIP_NONE);
}