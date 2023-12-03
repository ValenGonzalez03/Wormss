#include "client_beam.h"

Beam::Beam(Position &pos, std::vector<SDL2pp::Texture *> &beam_texture,
    SDL2pp::Renderer &rend, int angle)
    : position(pos), texture(beam_texture.front()), renderer(rend),
    angle(angle) {}

void Beam::render (int frame) {
  texture->SetAlphaMod(255);
  int sbeam_height = texture->GetHeight();
  int sbeam_width = texture->GetWidth();
  renderer.Copy(
      *texture, SDL2pp::Rect(0, 0, sbeam_width - 10, sbeam_height), // Size
      SDL2pp::Rect(position.get_position_x(), position.get_position_y(),
                    sbeam_width + 20, sbeam_height),
                    -angle,
                    pivot, // rotation center - not needed
                    SDL_FLIP_NONE);
}