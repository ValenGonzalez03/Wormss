#include "game_manager.h"
#include "box2d/box2d.h"
#include <stdio.h>

// cambiar despues
#define LEFT 0
#define RIGHT 1

GameManager::GameManager() {}

void GameManager::initialize_game() {
  world.create_beam(0, 1);
  world.create_beam(6,1,30);
  //world.create_beam(8,1,2);
  worm = world.create_worm(0, 7);
  worms_list.push_back(worm);
  // worms_list.push_back(world.create_worm(1, 4));
}

void GameManager::add_player(const int &player_id) {
  current_players++;
  players.push_back(player_id);
}

void GameManager::delete_player(const int &player_id) {
  current_players--;
  players.remove(player_id);
}

void GameManager::step() {
  world.step(timeStep, velocityIterations, positionIterations);
}

void GameManager::update() {
  worm->update();
}

void GameManager::move(const uint8_t &player_id, const uint8_t &direction) {
  // obtain worm
  /*
  if (direction == LEFT) {
    worm->move_left();
  } else if (direction == RIGHT) {
    worm->move_right();
  }*/
  
  worm->start_moving(direction);
}

void GameManager::stop_moving() {
  // obtain worm
  worm->stop_moving();
}

GameState GameManager::get_state() {
  GameState game_state;
  for (auto &current_worm: worms_list) {
	  game_state.add_worm(current_worm->get_pos_x(), current_worm->get_pos_y(), current_worm->get_direction(), current_worm->get_state()); //CAMBIAR LUEGO
  }
    
  return game_state;
}

// SOLO DE PRUEBA
b2Vec2 GameManager::get_worm_position() { return worm->get_position(); }

GameManager::~GameManager() {}
