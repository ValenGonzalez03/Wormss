#ifndef MORTAR_H
#define MORTAR_H

#include "weapon.h"

class Mortar : public Weapon {
    explicit Mortar(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, true, false, true, false, false, true, 10, 50, 2, 10, 6) {}
};

#endif
