#include "client_worldview.h"
#include "../../common/game_constants.h"

WorldView::WorldView(ResourcePool &res_pool, SDL2pp::Renderer &rend, Camera& camera)
    : resource_pool(res_pool), renderer(rend), worms(), camera(camera) {}

void WorldView::add_beam(float pos_x, float pos_y, float width, float height, float angle) {
  int pos_x_px = convert_meters_to_pixels_x(pos_x - width / 2);
  int pos_y_px = convert_meters_to_pixels_y(pos_y + height / 2);
  int width_px = convert_meters_to_pixels_x(width);
  int height_px = convert_meters_to_pixels_x(height);

  float angle_deg = angle * (180.0f / M_PI);

  std::vector<SDL2pp::Texture *> beam_texture =
      resource_pool.get_long_beam_texture();
  if (width == 6) {
    beam_texture = resource_pool.get_long_beam_texture();
  } else {
    beam_texture = resource_pool.get_short_beam_texture();
  }

  Beam beam(width_px, height_px, pos_x_px, pos_y_px, angle_deg, beam_texture,
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
  float width = WORM_WIDTH;
  float heigth = WORM_HEIGHT;

  int pos_x_px = convert_meters_to_pixels_x(data.get_pos_x() - width / 2);
  int pos_y_px = convert_meters_to_pixels_y(data.get_pos_y() + heigth / 2);
  int width_px = convert_meters_to_pixels_x(width);
  int heigth_px = convert_meters_to_pixels_x(heigth);

  Worm worm(data.get_player_id(), pos_x_px, pos_y_px, width_px, heigth_px, data.get_aim_angle(), data.get_direction(), 
  data.get_state(), renderer, resource_pool);
  worms.insert({worm.get_id(), worm});
}

Explodable WorldView::add_explodable(ExplodableData data) {
  SDL2pp::Texture* explodable_texture;
  switch (data.get_type())
  {
  case MISSILE:
    explodable_texture = resource_pool.get_missile_texture()[8];
    break;
  case GRENADE_BODY:
    explodable_texture = resource_pool.get_grenade_texture()[0];
    break;
  default:
    break;
  }

  auto size = get_explodable_size(data.get_type());
  float width = size.first;
  float heigth = size.second;

  int pos_x_px = convert_meters_to_pixels_x(data.get_pos_x() - width / 2);
  int pos_y_px = convert_meters_to_pixels_y(data.get_pos_y() + heigth / 2);
  int width_px = convert_meters_to_pixels_x(width);
  int heigth_px = convert_meters_to_pixels_x(heigth);


  Explodable explodable(pos_x_px, pos_y_px, width_px, heigth_px, data.get_angle(), data.get_direction(), data.get_id(),
                  std::move(explodable_texture), renderer);
  return explodable;
}

std::pair<float, float> WorldView::get_explodable_size(BODY_TYPES type) {
  float width;
  float height;

  switch (type) {
    case MISSILE:
      width = MISSILE_WIDTH;
      height = MISSILE_HEIGHT;
      break;
    case GRENADE_BODY:
      width = GRENADE_WIDTH;
      height = GRENADE_HEIGHT;
      break;
    default:
      std::cout << "No se encontro el body correspondiente" << std::endl;
      width = MISSILE_WIDTH;
      height = MISSILE_HEIGHT;
      break;
  }

  return {width, height};
}

void WorldView::add_explosion(ExplosionData data, int frame) {
  int pos_x_px = convert_meters_to_pixels_x(data.get_pos_x());
  int pos_y_px = convert_meters_to_pixels_y(data.get_pos_y());
  int radius_px = convert_meters_to_pixels_x(data.get_radius());

  Explosion explosion (pos_x_px, pos_y_px, radius_px, data.get_rays_fraction(), frame, renderer);
  explosions.emplace_back(explosion);
}

void WorldView::render(int frame) {
  //int cam_y = 0;
  // Renderizar fondo
  render_background();

  // Renderizar vigas
  for (auto &beam : beams) {
    beam.render(frame, camera.get_x(), camera.get_y());
  }

  // Renderizar gusanos
  for (auto &worm : worms) {
    worm.second.render(frame, camera.get_x(), camera.get_y());
  }

  // Renderizar misiles
  for (auto &explodable : explodables) {
    explodable.second.render(frame, camera.get_x(), camera.get_y());
  }

  // Renderizar explosiones
  for (auto explosion: explosions) {
    explosion.render(frame, camera.get_x(), camera.get_y());
  }

  // render_text("Position: " + std::to_string((int)state.position)
  //       + ", running: " + (state.is_running ? "true" : "false")
  //       + ", direction: " + std::to_string(int(state.direction)), "Vera", 0,
  //       0);
}

void WorldView::update(GameState &game_state, int frame) {
  camera.update();

  auto worms_data = game_state.get_worms();
  for (auto &worm : worms) {
    worm.second.update(worms_data[worm.second.get_id()]);
  }

  auto explodables_data = game_state.get_explodables();
  auto explodables_data_aux = explodables_data;
  for (auto explodable : explodables) {
    if (explodables_data_aux.find(explodable.first) == explodables_data_aux.end()) { // No encontró el elemento en missiles_data, quiere decir que el misil explotó (o desaparecio (?))
      explodables.erase(explodable.first);
    } else { // En otro caso lo encontró (Es un misil que sigue viajando y no chocó con nada), por lo que lo descarto de missiles_data
      explodables_data_aux.erase(explodable.first);
    }
  }
  for (auto data : explodables_data_aux) { // Itero sobre los misiles que quedan de missiles_data, que son los que NO tiene missiles por lo que debo crearlos.
    auto missile = add_explodable(data.second);
    explodables.insert({data.first, missile});
  }

  for (auto &missile : explodables) {
    missile.second.update(explodables_data[missile.second.get_id()]);
  }

  explosions.clear();
  auto explosions_data = game_state.get_explosions();
  for (auto data : explosions_data) {
    add_explosion(data, frame);
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

void WorldView::render_text(WormData &worm_data) {
  SDL2pp::Font font(RESOURCES_PATH "/Vera.ttf", 12);

  std::string text =
      "Pos x: " +
      std::to_string(worm_data.get_pos_x()) +
      ", Pos y: " +
      std::to_string(worm_data.get_pos_y() - WORM_HEIGHT) +
      ", state: " +
      (print_state(worm_data.get_state())) +
      ", weapon: " +
      (print_weapon_selected(worm_data.get_weapon_selected()));

  SDL2pp::Texture text_sprite(renderer, (font).RenderText_Blended(text, SDL_Color{255, 255, 255, 255}));

  renderer.Copy(text_sprite, SDL2pp::NullOpt,
      SDL2pp::Rect(0, 0, text_sprite.GetWidth(), text_sprite.GetHeight()));
  

  std::string dir = (worm_data.get_direction() == LEFT ? "left" : "right");
  std::string text_2 = "direction: " + dir +
      ", player_id: " + std::to_string(worm_data.get_player_id());

  SDL2pp::Texture text_sprite_2(renderer, (font).RenderText_Blended(text_2, SDL_Color{255, 255, 255, 255}));

  renderer.Copy(text_sprite_2, SDL2pp::NullOpt,
    SDL2pp::Rect(0, text_sprite.GetHeight(), text_sprite_2.GetWidth(), text_sprite_2.GetHeight()));
}

std::string WorldView::print_state(WormState state)
{
  switch (state)
  {
  case IDLE:
    return "idle";
  case MOVING:
    return "running";
  case JUMPING:
    return "jumping";
  case AIMING:
    return "aiming";
  case ATTACKING:
    return "attacking";
  default:
    return "unknown";
  }
}

std::string WorldView::print_weapon_selected(WeaponType weapon)
{
  switch (weapon)
  {
  case BAZOOKA:
    return "bazooka";
  case BAT:
    return "baseball bat";
  case GRENADE:
    return "grenade";
  default:
    return "unknown";
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