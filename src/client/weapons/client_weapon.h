#ifndef CLIENT_WEAPON_H
#define CLIENT_WEAPON_H

#include "../../common/game_constants.h"
#include "../client_resource_pool.h"
#include <vector>

class Weapon {
 private:
  WeaponType type;

 public:
  explicit Weapon(WeaponType type) : type(type) {}

  virtual std::vector<SDL2pp::Texture *> get_aim_textures(const ResourcePool &pool) = 0;

  virtual std::vector<SDL2pp::Texture *> get_attack_textures(
      const ResourcePool &pool) = 0;

  virtual texture_vals get_fixed_vals_aim(uint8_t worm_dir, texture_vals vals) = 0;

  virtual texture_vals get_fixed_vals_attack(uint8_t worm_dir, texture_vals vals) = 0;

  bool is_same_weapon(WeaponType other_type) { return type == other_type; }

  WeaponType get_type() const { return type; }

  virtual ~Weapon() = default;
};

#endif
