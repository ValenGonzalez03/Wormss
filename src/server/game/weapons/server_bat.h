#ifndef SERVER_BAT_H
#define SERVER_BAT_H

#include "server_weapon.h"

class ServerBat : public ServerWeapon {
 public:
  explicit ServerBat(WormBody* worm) : ServerWeapon(worm, BAT) {}
  void attack(World& world, float initial_force, int& proj_id_counter) override;
};

#endif
