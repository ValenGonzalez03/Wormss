#ifndef SERVER_GAME_H
#define SERVER_GAME_H

#include <algorithm>
#include <list>
#include <map>
#include <mutex>

#include <chrono>
#include <cmath>

#include "../common/game_state.h"
#include "../common/command.h"
#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/constant_rate_loop.h"
#include "command_runnable_game.h"
#include "game_manager.h"
#include "broadcaster.h"

#define MAX_PLAYERS 2
#define MS_PER_UPDATE 10
#define RATE 0.01

typedef duration<float, duration<float>> dur_ms;
typedef time_point<steady_clock, milliseconds> time_p_ms;
typedef duration<float> dur_f;

//class Command;

class Game : public Thread {
private:
  std::mutex m;
  std::map<int,std::shared_ptr<Queue<GameState>>> queues_sender;
  Broadcaster broadcaster;
  Queue<std::shared_ptr<RunnableCommandGame>> commands;
  uint8_t game_id;
  int last_player_id_added = 0;
  bool keep_playing = true;
  bool started = false;
  GameManager game_manager;
  std::chrono::duration<float> rate = std::chrono::duration<float>((float)RATE);
  
  

public:
  explicit Game(uint8_t &game_id);

  Queue<std::shared_ptr<RunnableCommandGame>>* add_player(std::shared_ptr<Queue<GameState>> sender_queue, 
											  const int& player_id);

  void delete_player(const int &player_id);

  void handle_command();

  void run() override;

  void update(int &it);

  void stop();

  bool compare_id(const uint8_t &another_game_id);

  void push_game_state();
  
  bool is_started();

  bool is_dead();
  
  int get_game_id();	//Provisional

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;
};

#endif

