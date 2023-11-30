#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "../common/game_state.h"
#include "box2d/box2d.h"
#include "world.h"
#include <stdio.h>
#include "worlds_reader.h"
#include "game_config.h"

class GameManager {
private:
  World world;
  int current_players = 0;
  int current_player_id;
  int current_worm_id;
  uint8_t current_turn_id = 0;
  std::list<uint8_t> players;
  //std::map <int, WormBody>
  std::list<WormBody*> worms_list;
	
  // PARA PRUEBAS
  //WormBody *worm;
	
  float timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;

  WormBody* get_worm(const uint8_t& player_id);

public:
  explicit GameManager();

  void initialize_game(GameConfig &game_config);

  void add_player(const uint8_t &player_id);

  void delete_player(const uint8_t &player_id);

  void set_current_turn_id(const uint8_t& id);

  void step();

  void update();

  void move(const uint8_t &player_id, const uint8_t &direction);
  
  void stop_moving(const uint8_t &player_id);

  void jump(const uint8_t &player_id, const uint8_t &direction);

  GameState get_state();

  // SOLO DE PRUEBA
  b2Vec2 get_worm_position();

  ~GameManager();

  GameManager(const GameManager &) = delete;
  GameManager &operator=(const GameManager &) = delete;
};

#endif
