#ifndef HOLY_GRANADE_H
#define HOLY_GRANADE_H

#include "weapon.h"

class HolyGranade : public Weapon {
    explicit HolyGranade(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, true, false, true, true, false, false, 2, 110, 8, 0, 0) {}
};

#endif
