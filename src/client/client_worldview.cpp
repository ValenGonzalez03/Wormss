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

void WorldView::render(int frame) { //Gamestate game_state
  //Renderizar fondo
  for (auto &beam : beams) {
    beam.render(frame);
  }
  //for (auto &worm : game_state.get_worm_list() {
  //  worm.render(frame);
  //}
  //Renderizar balas/cohetes

  //render_text("Position: " + std::to_string((int)state.position)
  //      + ", running: " + (state.is_running ? "true" : "false")
  //      + ", direction: " + std::to_string(int(state.direction)), "Vera", 0, 0);
}

/*
void WorldView::render_text(const std::string text, const std::string font_name, int pos_x, int pos_y) {
  SDL2pp::Texture text_sprite(renderer, (*resource_pool.get_font(font_name)).RenderText_Blended(text,
  SDL_Color{255, 255, 255, 255}));

  renderer.Copy(text_sprite, SDL2pp::NullOpt, SDL2pp::Rect(pos_x, pos_y,
  text_sprite.GetWidth(), text_sprite.GetHeight()));
}
*/