#ifndef CLIENT_WEAPON_H
#define CLIENT_WEAPON_H

#include "../../common/game_constants.h"
#include "../client_resource_pool.h"

class Weapon {
private:
    WeaponType type;

public:

    Weapon(WeaponType type) : type(type) {}

    virtual std::vector<SDL2pp::Texture *> get_aim_textures(ResourcePool &pool) = 0;

    virtual std::vector<SDL2pp::Texture *> get_attack_textures(ResourcePool &pool) = 0;

    virtual texture_vals get_fixed_vals_aim(uint8_t worm_dir, texture_vals vals) = 0;

    virtual texture_vals get_fixed_vals_attack(uint8_t worm_dir, texture_vals vals) = 0;

    bool is_same_weapon(WeaponType other_type) {
        return type == other_type;
    }

};

#endif