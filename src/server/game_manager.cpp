#include "game_manager.h"
#include "box2d/box2d.h"
#include <algorithm>
#include <stdio.h>

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

void GameManager::shoot(const uint8_t &player_id, float initial_force) {
  // if (player_id != current_turn_id) {
  //   return;
  // }
  WormBody *worm = world.get_worm(player_id);

  auto missile = worm->shoot(initial_force, missiles_id_counter);
  missiles_id_counter++;
  world.create_missile(missile);
  missile->apply_initial_impulse(worm->get_aiming_angle());
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

  return game_state;
}

void GameManager::set_game_finished(const bool is_finished) {
  game_finished = is_finished;
}

bool GameManager::is_game_finished() const {
  return game_finished;
}
