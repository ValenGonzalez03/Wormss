#include "server_game.h"
#include "../world/trajectory_missile_callback.h"
#include "../world/baseball_bat_callback.h"
#include "box2d/box2d.h"
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>

#define BAT_LENGTH 3
const float TIME_STEP = (1.0f / FPS);
const int32 VEL_ITERATIONS = 6;
const int32 POS_ITERATIONS = 2;

Game::Game(const World &world) : world(world) {}

void Game::charge_world(const GameConfig &game_config) {
  int i = 0;
  std::vector<std::vector<float>> spawn_points = world.get_spawn_points();
  for (uint8_t player_id : players) {
    int num_sp = i;
    if (i >= spawn_points.size()) {
      num_sp = spawn_points.size() - 1;
    }
    world.create_worm(player_id, spawn_points[num_sp][0], spawn_points[num_sp][1],
                      game_config);
    auto string = "[CLIENT-MAN-THREAD]: Worm of id: " +
                  std::to_string(static_cast<int>(player_id)) + " created.\n";
    std::cout << string;
    i++;
  }
}

void Game::add_player(const uint8_t &player_id) {
  current_players++;
  players.push_back(player_id);
}

void Game::delete_player(const uint8_t &player_id) {
  current_players--;
  players.remove(player_id);
}

// void Game::set_current_turn_id(const uint8_t &id) {
//   current_turn_id = id;
// }

World *Game::get_world() { return &world; }

void Game::update() {
  world.update_worms();
  world.update_explodables();
  world.update_explosions();

  world.step(TIME_STEP, VEL_ITERATIONS, POS_ITERATIONS);
}

void Game::move(const uint8_t &player_id, const uint8_t &direction) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->start_moving(direction);
}

void Game::stop_moving(const uint8_t &player_id) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->stop_moving();
}

void Game::jump(const uint8_t &player_id, const uint8_t &direction,
                const uint8_t &jump_type) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->jump(direction, jump_type);
}

void Game::aim(const uint8_t &player_id, const uint8_t &direction) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->start_aiming(direction);
}

void Game::stop_aiming(const uint8_t &player_id) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = world.get_worm(player_id);
  worm->stop_aiming();
}

void Game::change_weapon(const uint8_t &player_id, const uint8_t &weapon_type) {
  // if (player_id != current_turn_id) {
  //   return;
  // }
  WormBody *worm = world.get_worm(player_id);

  worm->change_weapon(static_cast<WeaponType>(weapon_type));
}

void Game::attack(const uint8_t &player_id, float initial_force) {
  // if (player_id != current_turn_id) {
  //   return;
  // }
  WormBody *worm = world.get_worm(player_id);

  if (worm->get_state() == ATTACKING)
    return;

  WeaponType weapon = worm->get_weapon_selected();
  switch (weapon) {
    case BAZOOKA:
      use_bazooka(worm, initial_force);
      break;
    case BAT:
      use_bat(worm);
      break;
    case GRENADE:
      use_grenade(worm, initial_force);
      break;
    default:
      /* Ningun arma (?) */
      break;
  }
  worm->set_worm_to_attack();
}

void Game::use_bazooka(WormBody *worm, float initial_force) {
  b2Vec2 missile_pos = worm->calculate_projectile_launch_position(
      MISSILE_WIDTH, MISSILE_HEIGHT, 0.27f, 0.27f);
  b2Vec2 worm_pos = b2Vec2(worm->get_pos_x(), worm->get_pos_y());
  MissileCallback callback;
  world.ray_cast(&callback, worm_pos, missile_pos);

  if (callback.did_hit_wall()) {
    b2Vec2 hit_point = callback.get_hit_point();
    world.create_explosion(hit_point.x, hit_point.y);
  } else {
    ExplodableAttr proj_attr =
        worm->attack_projectile(missile_pos, projectiles_id_counter);
    projectiles_id_counter++;
    world.create_missile(proj_attr.id, proj_attr.pos_x, proj_attr.pos_y, proj_attr.angle,
                         proj_attr.direction, initial_force);
  }
}

void Game::use_bat(WormBody *worm) {
  b2Vec2 worm_pos = b2Vec2(worm->get_pos_x(), worm->get_pos_y());
  UserData *data = worm->get_user_data();
  BaseballBatCallback bat_callback(worm_pos, data);

  float aim_angle = worm->get_aiming_angle();
  uint8_t worm_dir = worm->get_direction();
  float bat_end_x = (worm_dir == RIGHT ? 1 : -1) * cosf(aim_angle);
  b2Vec2 bat_end_pos = worm_pos + (BAT_LENGTH * b2Vec2(bat_end_x, sinf(aim_angle)));
  b2Vec2 dir = bat_end_pos - worm_pos;
  dir.Normalize();
  std::cout << "Bat direction: (" << dir.x << ", " << dir.y << ")" << std::endl;
  world.ray_cast(&bat_callback, worm_pos, bat_end_pos);
}

void Game::use_grenade(WormBody *worm, float initial_force) {
  b2Vec2 grenade_pos =
      worm->calculate_projectile_launch_position(GRENADE_WIDTH, GRENADE_HEIGHT);
  ExplodableAttr proj_attr = worm->attack_projectile(grenade_pos, projectiles_id_counter);
  projectiles_id_counter++;
  world.create_grenade(proj_attr.id, proj_attr.pos_x, proj_attr.pos_y, proj_attr.angle,
                       proj_attr.direction, initial_force);
}

GameState Game::create_state() {
  GameState game_state;
  if (game_finished) {
    game_state.set_game_finished();
  }

  auto worms_attr = world.get_worms_attr();
  for (const auto &attr : worms_attr) {
    game_state.add_worm(attr);
  }

  auto explodables_attr = world.get_explodables_attr();
  for (const auto &attr : explodables_attr) {
    game_state.add_explodable(attr);
  }

  auto explosions_attr = world.get_explosions_attr();
  for (const auto &attr : explosions_attr) {
    game_state.add_explosion(attr);
  }

  return game_state;
}

void Game::set_game_finished(const bool is_finished) { game_finished = is_finished; }

bool Game::is_game_finished() const { return game_finished; }
