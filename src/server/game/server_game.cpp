#include "server_game.h"
#include "../world/trajectory_missile_callback.h"
#include "../world/baseball_bat_callback.h"
#include "box2d/box2d.h"
#include <algorithm>
#include <stdio.h>
#include <string>
#include <vector>

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

void Game::set_worm_to_charge(const uint8_t &player_id) {
  // if (player_id != current_turn_id) {
  //   return;
  // }
  WormBody *worm = world.get_worm(player_id);

  worm->set_to_charge();
}

void Game::attack(const uint8_t &player_id, float charge_intensity) {
  // if (player_id != current_turn_id) {
  //   return;
  // }
  WormBody *worm = world.get_worm(player_id);

  if (worm->get_state() == ATTACKING)
    return;

  ServerWeapon *weapon = worm->get_weapon_selected();
  weapon->attack(world, charge_intensity, projectiles_id_counter);

  worm->set_to_attack();
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
