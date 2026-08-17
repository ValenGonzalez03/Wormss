#include "server_bazooka.h"
#include "../../bodies/worm_body.h"
#include "../../world/server_world.h"
#include "../../world/trajectory_missile_callback.h"
#include "../../../common/game_constants.h"

void ServerBazooka::attack(World& world, float initial_force, int& proj_id_counter) {
  b2Vec2 missile_pos = worm->calculate_projectile_launch_position(
      MISSILE_WIDTH, MISSILE_HEIGHT, 0.27f, 0.27f);
  b2Vec2 worm_pos = b2Vec2(worm->get_pos_x(), worm->get_pos_y());

  MissileCallback callback;
  world.ray_cast(&callback, worm_pos, missile_pos);

  if (callback.did_hit_wall()) {
    b2Vec2 hit_point = callback.get_hit_point();
    world.create_explosion(hit_point.x, hit_point.y);
  } else {
    ExplodableAttr proj_attr = worm->attack_projectile(missile_pos, proj_id_counter);
    proj_id_counter++;
    world.create_missile(proj_attr.id, proj_attr.pos_x, proj_attr.pos_y, proj_attr.angle,
                         proj_attr.direction, initial_force);
  }
}
