#ifndef BANANA_H
#define BANANA_H

#include "weapon.h"

class Banana : public Weapon {
    explicit Banana(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, true, false, true, true, false, false, 5, 70, 4, 0, 0) {}
};

#endif
