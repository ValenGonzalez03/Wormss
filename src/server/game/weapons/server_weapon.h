#ifndef SERVER_WEAPON_H
#define SERVER_WEAPON_H

#include "../../../common/game_constants.h"

class WormBody;
class World;

class ServerWeapon {
 protected:
  WormBody* worm;
  WeaponType type;

 public:
  explicit ServerWeapon(WormBody* worm, WeaponType type) : worm(worm), type(type) {}

  virtual void attack(World& world, float initial_force,
                      int& proj_id_counter) = 0;  // NOLINT(runtime/references)

  WeaponType get_type() { return type; }

  virtual ~ServerWeapon() = default;

  ServerWeapon(const ServerWeapon&) = delete;
  ServerWeapon& operator=(const ServerWeapon&) = delete;
};

#endif
