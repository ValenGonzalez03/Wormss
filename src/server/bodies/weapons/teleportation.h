#ifndef TELEPORTATION_H
#define TELEPORTATION_H

#include "weapon.h"

class Teleportation : public Weapon {
    explicit Teleportation(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, false, false, false, false, true, false, 10000, 0, 0, 0, 0) {}
};

#endif
