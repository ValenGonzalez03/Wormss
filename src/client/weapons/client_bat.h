#ifndef CLIENT_BAT_H
#define CLIENT_BAT_H

#include "client_weapon.h"

class Bat : public Weapon {
 private:
  WeaponType type;

 public:

  Bat(WeaponType type) : Weapon(type) {}

  std::vector<SDL2pp::Texture *> get_aim_textures(ResourcePool &pool) override;

  std::vector<SDL2pp::Texture *> get_attack_textures(ResourcePool &pool) override;

  texture_vals get_fixed_vals_aim(uint8_t worm_dir, texture_vals vals) override;

  texture_vals get_fixed_vals_attack(uint8_t worm_dir, texture_vals vals) override;
};

#endif