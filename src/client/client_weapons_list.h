#ifndef CLIENT_WEAPONS_LIST_H_
#define CLIENT_WEAPONS_LIST_H_

#include <vector>
#include "client_resource_pool.h"

#define FIRST_BAZ_TEXT 0
#define FIRST_BAT_TEXT 1

class WeaponTextureList {
private:
  std::vector<std::vector<SDL2pp::Texture *>> weapon_textures;

public:
  WeaponTextureList(ResourcePool &res_pool) {
    weapon_textures.push_back(res_pool.get_worm_aiming(BAZOOKA));
    weapon_textures.push_back(res_pool.get_worm_aiming(BAT));
    weapon_textures.push_back(res_pool.get_worm_attacking(BAT));
  }

  std::vector<SDL2pp::Texture*> get_aim_texture(WeaponType type) {
    switch (type) {
      case BAZOOKA:
        return weapon_textures[FIRST_BAZ_TEXT];
      case BAT:
        return weapon_textures[FIRST_BAT_TEXT];
      default:
        throw std::runtime_error("Weapon type not supported");
    }
  }

  std::vector<SDL2pp::Texture*> get_attack_texture(WeaponType type) {
    switch (type) {
      case BAZOOKA:
        return weapon_textures[FIRST_BAZ_TEXT];
      case BAT:
        return weapon_textures[FIRST_BAT_TEXT + 1];
      default:
        throw std::runtime_error("Weapon type not supported");
    }
  }
};

#endif