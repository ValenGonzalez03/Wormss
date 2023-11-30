#ifndef BASEBALL_BAT_H
#define BASEBALL_BAT_H

#include "weapon.h"

class BaseballBat : public Weapon {
    explicit BaseballBat(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, true, true, false, false, false, false, 10000, 10, 0, 0, 0) {}
};

#endif
