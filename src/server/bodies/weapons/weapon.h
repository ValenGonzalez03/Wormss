#ifndef WEAPON_H
#define WEAPON_H

#include "../body.h"

class Weapon : public Body {
protected:
    bool gunsight;
    bool melee;
    bool variable_power;
    bool countdown;
    bool remote_controlled;
    bool wind_sensitive;
    int ammo;
    int damage;
    int radius;
    int damage_per_fragment;
    int fragments;

public:
  explicit Weapon(b2World* world, float pos_x, float pos_y, bool gunsight, bool melee, bool variable_power, bool countdown, bool remote_controlled,
  bool wind_sensitive, int ammo, int damage, int radius, int damage_per_fragment, int fragments)
  : Body(world, pos_x, pos_y, 0, 0.2, 0.2, 1, 1), gunsight(gunsight), melee(melee), variable_power(variable_power), countdown(countdown), remote_controlled(remote_controlled),
  wind_sensitive(wind_sensitive), ammo(ammo), damage(damage), radius(radius), damage_per_fragment(damage_per_fragment), fragments(fragments) {
    // construir body del arma
  };
  
  Weapon(const Weapon&) = delete;
  Weapon& operator=(const Weapon&) = delete;
};

#endif
