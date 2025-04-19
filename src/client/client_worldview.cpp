#include "client_worldview.h"

WorldView::WorldView(ResourcePool &res_pool, SDL2pp::Renderer &rend)
    : resource_pool(res_pool), renderer(rend) {}

// void WorldView::add_short_beam(int pos_x, int pos_y, int angle) {
//   PositionConverter converter;
//   Position pos_in_m(pos_x, pos_y);
//   Position beam_pos = converter.convert_position_to_px(pos_in_m);
//   std::vector<SDL2pp::Texture *> beam_texture =
//       resource_pool.get_short_beam_texture();
//   Beam beam(beam_pos, beam_texture, renderer, angle);
//   beams.push_back(beam);
// }

void WorldView::add_beam(float pos_x, float pos_y, float width, float height,
                         int angle) {
  int pos_x_px = convert_meters_to_pixels_x(pos_x - width / 2);
  int pos_y_px = convert_meters_to_pixels_y(pos_y + height / 2);
  int width_px = convert_meters_to_pixels_x(width);
  int height_px = convert_meters_to_pixels_x(height);
  // std::cout << "Largo: " << width << std::endl;
  // std::cout << "Alto: " << height << std::endl;
  // std::cout << "Pos x (px): " << pos_x_px << std::endl;
  // std::cout << "Pos y (px): " << pos_y_px << std::endl;
  std::vector<SDL2pp::Texture *> beam_texture =
      resource_pool.get_long_beam_texture();
  if (width == 6) {
    beam_texture = resource_pool.get_long_beam_texture();
  } else {
    beam_texture = resource_pool.get_short_beam_texture();
  }

  Beam beam(width_px, height_px, pos_x_px, pos_y_px, angle, beam_texture,
            renderer);
  beams.push_back(beam);
}

void WorldView::render(int frame,
                       client_state &worm_state) { // Gamestate game_state
  auto walking_textures = resource_pool.get_worm_walking();
  auto jumping_textures = resource_pool.get_worm_jumping();
  auto aiming_textures = resource_pool.get_worm_aiming();
  WormView worm_view(renderer, walking_textures, jumping_textures,
                     aiming_textures); // Ver de hacer una sola instancia
  // Renderizar fondo
  render_background();
  for (auto &beam : beams) {
    beam.render(frame);
  }
  for (auto &worm : worms) {
    worm_view.render(frame, worm.second, worm_state);
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

void WorldView::set_background(const std::string &path) {
  resource_pool.add_background(path);
}

void WorldView::render_background() {
  std::shared_ptr<SDL2pp::Texture> background = resource_pool.get_background();
  if (background) {
    renderer.Copy(*background,
                  SDL2pp::Rect(0, 0, background->GetWidth(),
                               background->GetHeight()), // Size
                  SDL2pp::Rect(0, 0, renderer.GetOutputWidth(),
                               renderer.GetOutputHeight()), // Destination
                  0.0,                                      // don't rotate
                  SDL2pp::NullOpt, // rotation center - not needed
                  SDL_FLIP_NONE);  // horizontal flip
  } else {
    throw std::runtime_error("No existe el background");
  }
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