#include "client_bat.h"

std::vector<SDL2pp::Texture *> Bat::get_aim_textures(const ResourcePool &pool) { return pool.get_worm_aiming(BAT); }

std::vector<SDL2pp::Texture *> Bat::get_attack_textures(const ResourcePool &pool) {
  return pool.get_worm_attacking(BAT);
}

texture_vals Bat::get_fixed_vals_aim(uint8_t worm_dir, texture_vals vals) {
  int offset_x = worm_dir == RIGHT ? -10 : 0;
  return vals + texture_vals{offset_x, -17, 14, 34};
}

texture_vals Bat::get_fixed_vals_attack(uint8_t worm_dir, texture_vals vals) {
  int offset_x = worm_dir == RIGHT ? -2 : -14;
  return vals + texture_vals{offset_x, -17, 16, 34};
}
