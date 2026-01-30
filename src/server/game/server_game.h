#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <algorithm>
#include <list>
#include <map>
#include <mutex>

#include "math.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <memory>

#include "../../common/commands/command.h"
#include "../../common/constant_rate_loop.h"
#include "../../common/game_state.h"
#include "../../common/lib/queue.h"
#include "../../common/lib/thread.h"
#include "../comm/broadcaster.h"
#include "game_manager.h"
#include "../runnable_commands/command_runnable_game.h"

#define MAX_PLAYERS 2
#define MS_PER_UPDATE 10
#define RATE 0.01

typedef duration<float, duration<float>> dur_ms;
typedef time_point<steady_clock, milliseconds> time_p_ms;
typedef duration<float> dur_f;

class Game : public Thread {
 private:
  std::mutex m;
  Broadcaster broadcaster;
  Queue<std::shared_ptr<RunnableCommandGame>> commands;
  uint8_t game_id;
  uint8_t players_counter = 0;
  uint8_t current_turn_id = 0;
  bool keep_playing = true;
  bool started = false;
  GameManager game_manager;
  std::chrono::duration<float> rate = std::chrono::duration<float>((float)RATE);  // NOLINT(readability/casting)

  const GameConfig &config;

 public:
  explicit Game(uint8_t game_id, const GameConfig &game_config, const World &world);

  void add_player(std::shared_ptr<Queue<GameState>> sender_queue, const uint8_t &player_id);

  void delete_player(const uint8_t &player_id);

  void handle_command();

  void charge_world();

  void run() override;

  void update();

  void stop_playing();

  bool compare_id(const uint8_t &another_game_id);

  World get_world();

  void push_game_state();

  void check_game_finished();

  void turn_to_started();

  bool is_started();

  bool is_dead();

  uint8_t get_game_id();

  Queue<std::shared_ptr<RunnableCommandGame>> &get_commands_queue();

  GameManager &get_game_manager();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
};

#endif
