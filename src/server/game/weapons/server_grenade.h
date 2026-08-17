#ifndef SERVER_GRENADE_H
#define SERVER_GRENADE_H

#include "server_weapon.h"

class ServerGrenade : public ServerWeapon {
 public:
  explicit ServerGrenade(WormBody* worm) : ServerWeapon(worm, GRENADE) {}
  void attack(World& world, float initial_force, int& proj_id_counter) override;
};

#endif
