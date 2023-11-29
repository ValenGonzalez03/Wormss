#include "client_worldview.h"

void WorldView::add_short_beam(int pos_x, int pos_y, int angle) {
  PositionConverter converter;
  Position pos_in_m(pos_x, pos_y);
  Position beam_pos = converter.convert_position_to_px(pos_in_m);
  std::vector<SDL2pp::Texture *> beam_texture =
      resource_pool.get_short_beam_texture();
  Beam beam(beam_pos, beam_texture, renderer, angle);
  beams.push_back(beam);
}

void WorldView::add_long_beam(int pos_x, int pos_y, int angle) {
  PositionConverter converter;
  Position pos_in_m(pos_x, pos_y);
  Position beam_pos = converter.convert_position_to_px(pos_in_m);
  std::vector<SDL2pp::Texture *> beam_texture =
      resource_pool.get_long_beam_texture();
  Beam beam(beam_pos, beam_texture, renderer, angle);
  beams.push_back(beam);
}

void WorldView::render(int frame,
                       client_state &worm_state) { // Gamestate game_state
  // Renderizar fondo
  auto walking_textures = resource_pool.get_worm_walking();
  WormView worm_view(renderer,
                     walking_textures); // Ver de hacer una sola instancia
  for (auto &beam : beams) {
    beam.render(frame);
  }
  for (auto &worm : worms) {
    worm_view.render(frame, worm, worm_state);
  }
  // Renderizar balas/cohetes

  // render_text("Position: " + std::to_string((int)state.position)
  //       + ", running: " + (state.is_running ? "true" : "false")
  //       + ", direction: " + std::to_string(int(state.direction)), "Vera", 0,
  //       0);
}

void WorldView::update(GameState &game_state) {
  this->worms = game_state.get_worms();
}

/*
void WorldView::render_text(const std::string text, const std::string font_name,
int pos_x, int pos_y) { SDL2pp::Texture text_sprite(renderer,
(*resource_pool.get_font(font_name)).RenderText_Blended(text, SDL_Color{255,
255, 255, 255}));

  renderer.Copy(text_sprite, SDL2pp::NullOpt, SDL2pp::Rect(pos_x, pos_y,
  text_sprite.GetWidth(), text_sprite.GetHeight()));
}
*/