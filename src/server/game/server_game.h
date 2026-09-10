#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include "../../common/game_state.h"
#include "box2d/box2d.h"
#include "../world/server_world.h"
#include "worlds_reader.h"
#include "game_config.h"
#include "turn_manager.h"
#include <stdio.h>
#include <vector>
#include <utility>

#define FPS 60.0


class Game {
  friend class GameState;

 private:
  World world;
  bool game_finished = false;
  int current_players = 0;
  // int current_player_id;
  // int current_worm_id;
  // uint8_t current_turn_id = 0;
  int projectiles_id_counter = 0;
  std::vector<uint8_t> players;
  TurnManager turn_manager;
  std::chrono::steady_clock::time_point last_tick = std::chrono::steady_clock::now();

  void update_turn();

 public:
  explicit Game(const World &world);

  void charge_world(const GameConfig &game_config);

  void add_player(const uint8_t &player_id);

  void delete_player(const uint8_t &player_id);

  // void set_current_turn_id(const uint8_t &id);

  World *get_world();

  void update();

  void move(const uint8_t &player_id, const uint8_t &direction);

  void stop_moving(const uint8_t &player_id);

  void jump(const uint8_t &player_id, const uint8_t &direction, const uint8_t &jump_type);

  void aim(const uint8_t &player_id, const uint8_t &direction);

  void stop_aiming(const uint8_t &player_id);

  void change_weapon(const uint8_t &player_id, const uint8_t &weapon_type);

  void set_worm_to_charge(const uint8_t &player_id);

  void attack(const uint8_t &player_id, float charge_intensity);

  GameState create_state();

  void set_game_finished(const bool is_finished);

  bool is_game_finished() const;

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
};

#endif
