#include "game_manager.h"
#include "../world/trajectory_missile_callback.h"
#include "../world/baseball_bat_callback.h"
#include "box2d/box2d.h"
#include <algorithm>
#include <stdio.h>

#define BAT_LENGTH 3

GameManager::GameManager() {}

void GameManager::initialize_game(GameConfig &game_config) {
  int i = 0;
  std::vector<std::vector<float>> spawn_points = world.get_spawn_points();
  for (uint8_t player_id : players) {
    WormBody *worm = world.create_worm(player_id, spawn_points[i][0], spawn_points[i][1], game_config);
    std::cout << "worm created" << std::endl;
    i++;
  }
}

void GameManager::add_player(const uint8_t &player_id) {
  current_players++;
  players.push_back(player_id);
}

void GameManager::delete_player(const uint8_t &player_id) {
  current_players--;
  players.remove(player_id);
}

// void GameManager::set_current_turn_id(const uint8_t &id) {
//   current_turn_id = id;
// }

void GameManager::set_world(World& selected_world) {
  world = selected_world;
}

World& GameManager::get_world() {
  return this->world;
}

void GameManager::step() {
  world.step(timeStep, velocityIterations, positionIterations);
}

void GameManager::update() {
  world.update_worms();

  world.update_missiles();

  world.update_explosions();
}

void GameManager::move(const uint8_t &player_id, const uint8_t &direction) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->start_moving(direction);
}

void GameManager::stop_moving(const uint8_t &player_id) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->stop_moving();
}

void GameManager::jump(const uint8_t &player_id, const uint8_t &direction, const uint8_t &jump_type) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->jump(direction, jump_type);
}

void GameManager::aim(const uint8_t &player_id, const uint8_t &direction) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->start_aiming(direction);
}

void GameManager::stop_aiming(const uint8_t &player_id) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->stop_aiming();
}

void GameManager::change_weapon(const uint8_t &player_id, const uint8_t &weapon_type) {
  // if (player_id != current_turn_id) {
  //   return;
  // }
  WormBody *worm = world.get_worm(player_id);

  worm->change_weapon(static_cast<WeaponType>(weapon_type));
}

void GameManager::attack(const uint8_t &player_id, float initial_force) {
  // if (player_id != current_turn_id) {
  //   return;
  // }
  WormBody *worm = world.get_worm(player_id);

  if (worm->get_state() == ATTACKING) return;

  WeaponType weapon = worm->get_weapon_selected();
  switch (weapon)
  {
  case BAZOOKA:
    use_bazooka(worm, initial_force);
    break;
  case BAT:
    use_bat(worm);
    break;
  default:
    /* Ningun arma (?) */
    break;
  }
  worm->set_worm_to_attack();
}

void GameManager::use_bazooka(WormBody* worm, float initial_force) {
  b2Vec2 missile_pos = worm->calculate_missile_launch_position();
  b2Vec2 worm_pos = worm->get_position();
  MissileCallback callback;
  world.ray_cast(&callback, worm_pos, missile_pos);

  if (callback.did_hit_wall()) {
    b2Vec2 hit_point = callback.get_hit_point();
    world.create_explosion(hit_point.x, hit_point.y);
  } else {
    MissileBody* missile = worm->attack_throwable(missile_pos, initial_force, missiles_id_counter);
    missiles_id_counter++;
    world.create_missile(missile);
    missile->apply_initial_impulse(worm->get_aiming_angle());
  }
}

void GameManager::use_bat(WormBody* worm) {
  b2Vec2 worm_pos = worm->get_position();
  UserData* data = worm->get_user_data();
  BaseballBatCallback bat_callback(worm_pos, data);

  float aim_angle = worm->get_aiming_angle();
  uint8_t worm_dir = worm->get_direction();
  float bat_end_x = (worm_dir == RIGHT ? 1 : -1) * cosf(aim_angle);
  b2Vec2 bat_end_pos = worm_pos + (BAT_LENGTH * b2Vec2( bat_end_x, sinf(aim_angle)));
  b2Vec2 dir = bat_end_pos - worm_pos;
  dir.Normalize();
  std::cout << "Bat direction: (" << dir.x << ", " << dir.y << ")" << std::endl;
  world.ray_cast(&bat_callback, worm_pos, bat_end_pos);
}

GameState GameManager::create_state() {
  GameState game_state;
  if (game_finished) {
    game_state.set_game_finished();
  }

  auto worms_attr = world.get_worms_attr();
  for (auto attr : worms_attr) {
    game_state.add_worm(attr);
  }

  auto missiles_attr = world.get_missiles_attr();
  for (auto attr : missiles_attr) {
    game_state.add_missile(attr);
  }

  auto explosions_attr = world.get_explosions_attr();
  for (auto attr : explosions_attr) {
    game_state.add_explosion(attr);
  }

  return game_state;
}

void GameManager::set_game_finished(const bool is_finished) {
  game_finished = is_finished;
}

bool GameManager::is_game_finished() const {
  return game_finished;
}
