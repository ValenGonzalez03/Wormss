#include "server_bat.h"
#include "../../bodies/worm_body.h"
#include "../../world/server_world.h"
#include "../../world/baseball_bat_callback.h"
#include "../../../common/game_constants.h"

#define BAT_LENGTH 3

void ServerBat::attack(World& world, float charge_intensity, int& proj_id_counter) {
  b2Vec2 worm_pos = b2Vec2(worm->get_pos_x(), worm->get_pos_y());
  UserData* data = worm->get_user_data();
  BaseballBatCallback bat_callback(worm_pos, data);

  float aim_angle = worm->get_aiming_angle();
  uint8_t worm_dir = worm->get_direction();
  float bat_end_x = (worm_dir == RIGHT ? 1 : -1) * cosf(aim_angle);
  b2Vec2 bat_end_pos = worm_pos + (BAT_LENGTH * b2Vec2(bat_end_x, sinf(aim_angle)));

  world.ray_cast(&bat_callback, worm_pos, bat_end_pos);
}
