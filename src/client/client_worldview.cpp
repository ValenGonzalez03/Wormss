#include "client_worldview.h"

void WorldView::add_short_beam(int pos_x, int pos_y) {
  Position beam_pos(pos_x, pos_y);
  SDL2pp::Texture &beam_texture = *resource_pool.get_short_beam_texture();
  Beam beam(beam_pos, beam_texture, renderer);
  beams.push_back(beam);
}

void WorldView::add_long_beam(int pos_x, int pos_y) {
  Position beam_pos(pos_x, pos_y);
  SDL2pp::Texture &beam_texture = *resource_pool.get_long_beam_texture();
  Beam beam(beam_pos, beam_texture, renderer);
  beams.push_back(beam);
}

void WorldView::render(int frame) {
  for (auto &beam : beams) {
    beam.render(frame);
  }
}