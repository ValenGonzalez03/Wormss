#include "client_worldview.h"

void WorldView::add_beam(Position beam_pos) {
    SDL2pp::Texture beam_texture = resource_pool.get_short_beam_texture();
    Beam beam(beam_pos, beam_texture);
}