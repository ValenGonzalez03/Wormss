#include "client_grenade.h"


std::vector<SDL2pp::Texture *> Grenade::get_aim_textures(ResourcePool &pool) { return pool.get_worm_aiming(GRENADE); }

std::vector<SDL2pp::Texture *> Grenade::get_attack_textures(ResourcePool &pool) {
  return pool.get_worm_aiming(GRENADE);
}

texture_vals Grenade::get_fixed_vals_aim(uint8_t worm_dir, texture_vals vals) {
  return vals + texture_vals{-4, -4, 8, 6};
}

texture_vals Grenade::get_fixed_vals_attack(uint8_t worm_dir, texture_vals vals) {
  return vals + texture_vals{-4, -4, 8, 6};
}
