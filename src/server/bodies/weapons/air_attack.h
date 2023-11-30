#ifndef AIR_ATTACK_H
#define AIR_ATTACK_H

#include "weapon.h"

class AirAttack : public Weapon {
    explicit AirAttack(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, false, false, false, false, true, true, 2, 40, 2, 0, 0) {}
};

#endif
