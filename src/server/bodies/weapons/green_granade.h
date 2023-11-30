#ifndef GREEN_GRANADE_H
#define GREEN_GRANADE_H

#include "weapon.h"

class GreenGranade : public Weapon {
    explicit GreenGranade(b2World* world, float pos_x, float pos_y) : Weapon(world, pos_x, pos_y, true, false, true, true, false, false, 10000, 30, 2, 0, 0) {}
};

#endif
