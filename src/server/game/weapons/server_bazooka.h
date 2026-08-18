#ifndef SERVER_BAZOOKA_H
#define SERVER_BAZOOKA_H

#include "server_weapon.h"

class ServerBazooka : public ServerWeapon {
 public:
  explicit ServerBazooka(WormBody* worm) : ServerWeapon(worm, BAZOOKA) {}
  void attack(World& world, float charge_intensity, int& proj_id_counter) override;
};

#endif
