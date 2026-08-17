#include "server_grenade.h"
#include "../../bodies/worm_body.h"
#include "../../world/server_world.h"
#include "../../../common/game_constants.h"

void ServerGrenade::attack(World& world, float initial_force, int& proj_id_counter) {
  b2Vec2 grenade_pos =
      worm->calculate_projectile_launch_position(GRENADE_WIDTH, GRENADE_HEIGHT);
  ExplodableAttr proj_attr = worm->attack_projectile(grenade_pos, proj_id_counter);
  proj_id_counter++;
  world.create_grenade(proj_attr.id, proj_attr.pos_x, proj_attr.pos_y, proj_attr.angle,
                       proj_attr.direction, initial_force);
}
