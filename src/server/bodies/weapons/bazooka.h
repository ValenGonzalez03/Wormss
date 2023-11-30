#ifndef BAZOOKA_H
#define BAZOOKA_H

#include "weapon.h"

class Bazooka : public Weapon {
    explicit Bazooka(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, true, false, true, false, false, true, 10000, 50, 2, 0, 0) {}
};

#endif
