#include "game_manager.h"
#include "box2d/box2d.h"
#include <stdio.h>

GameManager::GameManager() {}

void GameManager::initialize_game(GameConfig &game_config) {
  int i = 0;
  std::vector<std::vector<float>> spawn_points = world.get_spawn_points();
  for (uint8_t player_id : players) {
    std::cout << "worm created" << std::endl;
    WormBody *worm = world.create_worm(spawn_points[i][0], spawn_points[i][1],game_config.get_worm_speed(), game_config.get_worm_life(), player_id);
    worms_list.push_back(worm);
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

void GameManager::set_current_turn_id(const uint8_t &id) {
  current_turn_id = id;
}


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
  for (WormBody *worm : worms_list) {
    worm->update();
  }
}

WormBody *GameManager::get_worm(const uint8_t &player_id) {
  for (auto it = worms_list.begin(); it != worms_list.end(); ++it) {
    if ((*it)->get_id() == player_id) {
      return (*it);
    }
  }
  return nullptr;
}

void GameManager::move(const uint8_t &player_id, const uint8_t &direction) {
  if (player_id != current_turn_id) {
    return;
  }

  WormBody *worm = get_worm(player_id);
  worm->start_moving(direction);
}

void GameManager::stop_moving(const uint8_t &player_id) {
  if (player_id != current_turn_id) {
    return;
  }

  WormBody *worm = get_worm(player_id);
  worm->stop_moving();
}

void GameManager::jump(const uint8_t &player_id, const uint8_t &direction) {
  if (player_id != current_turn_id) {
    return;
  }

  WormBody *worm = get_worm(player_id);
  worm->jump(direction);
}

void GameManager::aim(const uint8_t &player_id, const uint8_t &direction) {
  if (player_id != current_turn_id) {
    return;
  }

  WormBody *worm = get_worm(player_id);
  worm->start_aiming(direction);
}

void GameManager::stop_aiming(const uint8_t &player_id) {
  if (player_id != current_turn_id) {
    return;
  }

  WormBody *worm = get_worm(player_id);
  worm->stop_aiming();
}

GameState GameManager::get_state() {
  GameState game_state;
  for (auto &current_worm : worms_list) {
    game_state.add_worm(current_worm->get_id(), current_worm->get_pos_x(),
                        current_worm->get_pos_y(),
                        current_worm->get_direction(),
                        current_worm->get_state(),
                        current_worm->get_aiming_angle());
  }

  return game_state;
}

GameManager::~GameManager() {}
