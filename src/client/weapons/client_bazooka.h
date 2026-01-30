#ifndef CLIENT_BAZOOKA_H
#define CLIENT_BAZOOKA_H

#include "client_weapon.h"
#include <vector>

class Bazooka : public Weapon {
 public:
  explicit Bazooka(WeaponType type) : Weapon(type) {}

  std::vector<SDL2pp::Texture *> get_aim_textures(const ResourcePool &pool) override;

  std::vector<SDL2pp::Texture *> get_attack_textures(const ResourcePool &pool) override;

  texture_vals get_fixed_vals_aim(uint8_t worm_dir, texture_vals vals) override;

  texture_vals get_fixed_vals_attack(uint8_t worm_dir, texture_vals vals) override;
};

#endif
