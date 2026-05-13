#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "../../common/game_state.h"
#include "box2d/box2d.h"
#include "../world/server_world.h"
#include "worlds_reader.h"
#include "game_config.h"
#include <stdio.h>
#include <list>
#include <utility>

class GameManager {
 private:
  World world;
  bool game_finished = false;
  int current_players = 0;
  // int current_player_id;
  // int current_worm_id;
  // uint8_t current_turn_id = 0;
  int projectiles_id_counter = 0;
  std::list<uint8_t> players;

  float timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 6;
  int32 positionIterations = 2;

  void use_bazooka(WormBody *worm, float initial_force);

  void use_bat(WormBody *worm);

  void use_grenade(WormBody *worm, float initial_force);

 public:
  explicit GameManager(const World &world);

  void initialize_game(const GameConfig &game_config);

  void add_player(const uint8_t &player_id);

  void delete_player(const uint8_t &player_id);

  // void set_current_turn_id(const uint8_t &id);

  World *get_world();

  void step();

  void update();

  void move(const uint8_t &player_id, const uint8_t &direction);

  void stop_moving(const uint8_t &player_id);

  void jump(const uint8_t &player_id, const uint8_t &direction, const uint8_t &jump_type);

  void aim(const uint8_t &player_id, const uint8_t &direction);

  void stop_aiming(const uint8_t &player_id);

  void change_weapon(const uint8_t &player_id, const uint8_t &weapon_type);

  void attack(const uint8_t &player_id, float initial_force);

  GameState create_state();

  void set_game_finished(const bool is_finished);

  bool is_game_finished() const;

  GameManager(const GameManager &) = delete;
  GameManager &operator=(const GameManager &) = delete;
};

#endif
