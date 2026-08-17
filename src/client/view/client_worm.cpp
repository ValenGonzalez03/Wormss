#include "client_worm.h"

#include <cmath>

Worm::Worm(uint8_t id, int pos_x, int pos_y, int width, int height, float aim_angle,
           uint8_t direction, WormState worm_state, SDL2pp::Renderer &rend,
           ResourcePool &res_pool) :
    id(id), pos_x(pos_x), pos_y(pos_y), width(width), height(height),
    aim_angle(aim_angle), direction(direction), worm_state(worm_state),
    resource_pool(res_pool), renderer(rend) {
  weapon = new Bazooka(BAZOOKA);
}

int Worm::get_id() { return id; }

void Worm::update(WormData data) {
  id = data.get_player_id();

  pos_x = convert_meters_to_pixels_x(data.get_pos_x()) - width / 2;
  pos_y = convert_meters_to_pixels_y(data.get_pos_y()) - height / 2;

  worm_state = data.get_state();
  direction = data.get_direction();
  aim_angle = data.get_aim_angle();

  WeaponType w_type = data.get_weapon_selected();
  update_weapon_selected(w_type);
}

void Worm::update_weapon_selected(WeaponType type) {
  if (weapon->is_same_weapon(type))
    return;

  assign_new_weapon(type);
}

void Worm::assign_new_weapon(WeaponType type) {
  if (weapon) {
    delete weapon;
  }

  switch (type) {
    case BAZOOKA:
      weapon = new Bazooka(BAZOOKA);
      break;
    case BAT:
      weapon = new Bat(BAT);
      break;
    case GRENADE:
      weapon = new Grenade(GRENADE);
      break;
    default:
      throw std::runtime_error("Weapon type not supported");
  }
}

void Worm::render(int frame, int camera_x, int camera_y) {
  if (this->worm_state == MOVING) {
    render_worm_running(frame, camera_x, camera_y);
  } else if (this->worm_state == AIMING) {
    render_worm_aiming(frame, camera_x, camera_y);
  } else if (this->worm_state == CHARGING_ATTACK) {
    render_worm_attacking(frame, camera_x, camera_y);
  } else if (this->worm_state == ATTACKING) {
    render_worm_attacking(frame, camera_x, camera_y);
  } else {
    render_worm_idle(frame, camera_x, camera_y);
  }

  if (std::getenv("DEBUG") != NULL) {
    SDL2pp::Rect box(pos_x - camera_x, pos_y - camera_y, width, height);

    SDL2pp::Color c(255, 0, 0);
    renderer.SetDrawColor(c);
    renderer.DrawRect(box);
  }
}

void Worm::render_worm_idle(int frame, int camera_x, int camera_y) {
  SDL_RendererFlip flip = choose_flip_direction();

  auto walk_texture = resource_pool.get_worm_walking();
  // std::cout << "LLEGO?" << std::endl;
  walk_texture.front()->SetAlphaMod(255);
  walk_texture.front()->SetBlendMode(SDL_BLENDMODE_BLEND);

  renderer.Copy(
      *walk_texture.front(), SDL2pp::NullOpt,
      SDL2pp::Rect(pos_x - camera_x, pos_y - 3 - camera_y, width + 2, height + 4), 0.0,
      SDL2pp::NullOpt, flip);
}

void Worm::render_worm_running(int frame, int camera_x, int camera_y) {
  SDL_RendererFlip flip = choose_flip_direction();

  auto walk_texture = resource_pool.get_worm_walking();
  auto normalized_frame = frame / 40;
  auto frame_position = normalized_frame % walk_texture.size();
  walk_texture[frame_position]->SetBlendMode(SDL_BLENDMODE_BLEND);
  walk_texture[frame_position]->SetAlphaMod(255);  // sprite is fully opaque

  renderer.Copy(
      *walk_texture[frame_position], SDL2pp::NullOpt,
      SDL2pp::Rect(pos_x - camera_x, pos_y - 3 - camera_y, width + 2, height + 4), 0.0,
      SDL2pp::NullOpt, flip);
}

void Worm::render_worm_jumping(int frame, int camera_x, int camera_y) {}

void Worm::render_worm_aiming(int frame, int camera_x, int camera_y) {
  SDL_RendererFlip flip = choose_flip_direction();

  auto aim_texture = weapon->get_aim_textures(resource_pool);

  auto normalized_angle = (aim_angle / M_PI_2);
  auto frame_position =
      16 + static_cast<int>(normalized_angle * ((aim_texture.size() / 2)));
  aim_texture[frame_position]->SetBlendMode(SDL_BLENDMODE_BLEND);
  aim_texture[frame_position]->SetAlphaMod(255);


  auto vals =
      weapon->get_fixed_vals_aim(direction, texture_vals{pos_x, pos_y, width, height});
  renderer.Copy(
      *aim_texture[frame_position], SDL2pp::NullOpt,
      SDL2pp::Rect(vals.x - camera_x, vals.y - camera_y, vals.width, vals.height), 0.0,
      SDL2pp::NullOpt, flip);
}

void Worm::render_worm_attacking(int frame, int camera_x, int camera_y) {
  SDL_RendererFlip flip = choose_flip_direction();

  auto attack_texture = weapon->get_attack_textures(resource_pool);

  auto normalized_angle = (aim_angle / M_PI_2);
  auto frame_position =
      16 + static_cast<int>(normalized_angle * ((attack_texture.size() / 2)));
  attack_texture[frame_position]->SetBlendMode(SDL_BLENDMODE_BLEND);
  attack_texture[frame_position]->SetAlphaMod(255);

  auto vals =
      weapon->get_fixed_vals_attack(direction, texture_vals{pos_x, pos_y, width, height});
  renderer.Copy(
      *attack_texture[frame_position], SDL2pp::NullOpt,
      SDL2pp::Rect(vals.x - camera_x, vals.y - camera_y, vals.width, vals.height), 0.0,
      SDL2pp::NullOpt, flip);
}

SDL_RendererFlip Worm::choose_flip_direction() {
  // Chequeo solo si la direccion es a la derecha ya que todos los sprites miran por defecto hacia la izquierda
  return (direction == RIGHT ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

int Worm::get_pos_x() { return pos_x; }
int Worm::get_pos_y() { return pos_y; }
int Worm::get_width() { return width; }
int Worm::get_height() { return height; }

Worm::~Worm() {
  // delete weapon;
}
