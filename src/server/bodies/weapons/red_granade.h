#ifndef RED_GRANADE_H
#define RED_GRANADE_H

#include "weapon.h"

class RedGranade : public Weapon {
    explicit RedGranade(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, true, false, true, true, false, false, 10, 30, 2, 10, 6) {}
};

#endif
