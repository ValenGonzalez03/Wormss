#include "game_manager.h"
#include "box2d/box2d.h"
#include <algorithm>
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
  for (std::list<MissileBody*>::iterator it = missiles_list.begin(); it != missiles_list.end();)
    {
      if ((*it)->has_exploded()) {
        world.destroy_missile(*it);
        it = missiles_list.erase(it);
        std::cout << "KABOOM" << std::endl;
      } else {
        (*it)->update();
        it++;
      }
    }
  // for (MissileBody *missile : missiles_list) {
  // }
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
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = get_worm(player_id);
  worm->start_moving(direction);
}

void GameManager::stop_moving(const uint8_t &player_id) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = get_worm(player_id);
  worm->stop_moving();
}

void GameManager::jump(const uint8_t &player_id, const uint8_t &direction, const uint8_t &jump_type) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = get_worm(player_id);
  worm->jump(direction, jump_type);
}

void GameManager::aim(const uint8_t &player_id, const uint8_t &direction) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = get_worm(player_id);
  worm->start_aiming(direction);
}

void GameManager::stop_aiming(const uint8_t &player_id) {
  // if (player_id != current_turn_id) {
  //   return;
  // }

  WormBody *worm = get_worm(player_id);
  worm->stop_aiming();
}

void GameManager::shoot(const uint8_t &player_id, float initial_force) {
  // if (player_id != current_turn_id) {
  //   return;
  // }
  WormBody *worm = get_worm(player_id);

  auto missile = worm->shoot(initial_force, missiles_id_counter);
  missiles_id_counter++;
  missiles_list.push_back(missile);
  missile->apply_initial_impulse(worm->get_aiming_angle());
}

GameState GameManager::create_state() {
  GameState game_state;
  if (game_finished) {
    game_state.set_game_finished();
  }
  for (auto &current_worm : worms_list) {
    game_state.add_worm(current_worm->get_id(), current_worm->get_pos_x(),
                        current_worm->get_pos_y(),
                        current_worm->get_direction(),
                        current_worm->get_state(),
                        current_worm->get_aiming_angle());
  }
  for (auto &current_missile : missiles_list) {
    game_state.add_missile(current_missile->get_pos_x(),
                           current_missile->get_pos_y(),
                           current_missile->get_angle(),
                          current_missile->get_id());
    //std::cout << "final_pos_x (missile): " << current_missile->get_pos_x() << std::endl;
    //std::cout << "final_pos_y (missile): " << current_missile->get_pos_y() - MISSILE_HEIGHT << std::endl;
    //std::cout << "angle (missile): " << current_missile->get_angle() << std::endl;
  }

  return game_state;
}

void GameManager::set_game_finished(const bool is_finished) {
  game_finished = is_finished;
}

bool GameManager::is_game_finished() const {
  return game_finished;
}
