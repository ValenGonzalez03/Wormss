#include "client_bazooka.h"


std::vector<SDL2pp::Texture *> Bazooka::get_aim_textures(const ResourcePool &pool) {
  return pool.get_worm_aiming(BAZOOKA);
}

std::vector<SDL2pp::Texture *> Bazooka::get_attack_textures(const ResourcePool &pool) {
  return pool.get_worm_attacking(BAZOOKA);
}

texture_vals Bazooka::get_fixed_vals_aim(uint8_t worm_dir, texture_vals vals) {
  return vals + texture_vals{-4, -2, 8, 2};
}

texture_vals Bazooka::get_fixed_vals_attack(uint8_t worm_dir, texture_vals vals) {
  return vals + texture_vals{-4, -2, 8, 2};
}
