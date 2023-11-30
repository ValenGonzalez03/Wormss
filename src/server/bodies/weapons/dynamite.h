#ifndef DYNAMITE_H
#define DYNAMITE_H

#include "weapon.h"

class Dynamite : public Weapon {
    explicit Dynamite(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, false, true, false, true, false, false, 5, 50, 4, 0, 0) {}
};

#endif
