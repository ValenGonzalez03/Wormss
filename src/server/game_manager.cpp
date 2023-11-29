#include "game_manager.h"
#include "box2d/box2d.h"
#include <stdio.h>

// cambiar despues
#define LEFT 0
#define RIGHT 1

GameManager::GameManager() {}

void GameManager::initialize_game() {
  /*
  WorldsReader worlds_reader;
  std::vector<std::shared_ptr<World>> worlds =
  worlds_reader.read_yaml_files(std::filesystem::path(RESOURCES_PATH) /
  "Worlds"); for (auto &world : worlds){ world->print_beams();
    world->print_worms();
  }
  */
  world.create_beam(0, 1, 0, 6);
  world.create_beam(6, 1, 30, 6);

  for (uint8_t player_id : players) {
    WormBody *worm = world.create_worm(0, 7, player_id);
    worms_list.push_back(worm);
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

void GameManager::step() {
  world.step(timeStep, velocityIterations, positionIterations);
}

void GameManager::update() {
  for (WormBody *worm : worms_list) {
    worm->update();
  }
  // WormBody* worm = get_worm(current_turn_id); // verificar si no le tiene que
  // pasar el player_id tambien worm->update();
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

GameState GameManager::get_state() {
  GameState game_state;
  for (auto &current_worm : worms_list) {
    game_state.add_worm(current_worm->get_pos_x(), current_worm->get_pos_y(),
                        current_worm->get_direction(),
                        current_worm->get_state()); // CAMBIAR LUEGO
  }

  return game_state;
}

// SOLO DE PRUEBA
b2Vec2 GameManager::get_worm_position() {
  return get_worm(current_turn_id)->get_position();
}

GameManager::~GameManager() {}
