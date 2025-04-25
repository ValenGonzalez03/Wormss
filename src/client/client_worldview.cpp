#include "client_worldview.h"
#include "../common/game_constants.h"

WorldView::WorldView(ResourcePool &res_pool, SDL2pp::Renderer &rend)
    : resource_pool(res_pool), renderer(rend), worms() {}

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

// void WorldView::add_worms(std::vector<std::vector<float>> spawn_points) {
//   for (int i = 0; i < worms.size(); i++) {
//       WormData data(-1, spawn_points[i][0], spawn_points[i][1], RIGHT, 0, 0);
//       add_worm(data);
//   }
// }

void WorldView::add_worm(WormData data) {
  std::vector<std::vector<SDL2pp::Texture *>> worm_textures;
  worm_textures.push_back(resource_pool.get_worm_walking());
  worm_textures.push_back(resource_pool.get_worm_aiming());
  //worm_textures.push_back(resource_pool.get_worm_jumping());

  float width = WORM_WIDTH;
  float heigth = WORM_HEIGHT;

  int pos_x_px = convert_meters_to_pixels_x(data.get_pos_x() - width / 2);
  int pos_y_px = convert_meters_to_pixels_y(data.get_pos_y() + heigth / 2);
  int width_px = convert_meters_to_pixels_x(width);
  int heigth_px = convert_meters_to_pixels_x(heigth);

  
  Worm worm(data.get_player_id(), pos_x_px, pos_y_px, width_px, heigth_px, data.get_aim_angle(), data.get_direction(), 
  data.get_state(), std::move(worm_textures), renderer);
  worms.insert({worm.get_id(), worm});
}

void WorldView::render(int frame) { // Gamestate game_state
  // Renderizar fondo
  render_background();
  // Renderizar vigas
  for (auto &beam : beams) {
    beam.render(frame);
  }

  // Renderizar gusanos
  for (auto &worm : worms) {
    worm.second.render(frame);
  }
  // Renderizar balas/cohetes

  // render_text("Position: " + std::to_string((int)state.position)
  //       + ", running: " + (state.is_running ? "true" : "false")
  //       + ", direction: " + std::to_string(int(state.direction)), "Vera", 0,
  //       0);
}

void WorldView::update(GameState &game_state) {
  auto worms_data = game_state.get_worms();
  for (auto &worm : worms) {
    worm.second.update(worms_data[worm.second.get_id()]);
  }
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