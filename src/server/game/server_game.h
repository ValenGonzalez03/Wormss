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
#include "../comm/player_sender_thread.h"
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
  uint8_t game_id;
  GameManager game_manager;
  const GameConfig &config;
  Queue<game_command_ptr> &commands;
  Broadcaster broadcaster;
  std::list<PlayerSender*> player_senders;

  uint8_t players_counter = 0;
  uint8_t current_turn_id = 0;
  bool keep_playing = true;
  bool started = false;

  std::mutex m;
  std::chrono::duration<float> rate = std::chrono::duration<float>((float)RATE);  // NOLINT(readability/casting)

 public:
  explicit Game(const uint8_t &game_id, const World &world, const GameConfig &game_config,
                Queue<game_command_ptr> &receiver_queue);  // NOLINT(runtime/references)

  void add_player(PlayerSender& sender, const uint8_t &player_id);  // NOLINT(runtime/references)

  void delete_player(const uint8_t &player_id);

  void handle_command();

  void charge_world();

  void run() override;

  void update();

  void send_info_to_start_to_players();

  void stop_playing();

  bool compare_id(const uint8_t &another_game_id);

  World* get_world();

  void push_game_state();

  void check_game_finished();

  void turn_to_started();

  bool is_started();

  bool is_dead();

  uint8_t get_game_id();

  Queue<game_command_ptr> &get_commands_queue();

  GameManager &get_game_manager();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
};

#endif
