#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <algorithm>
#include <list>
#include <mutex>

#include "command_runnable_game.h"
#include "server_game.h"

class Command;

class GamesHandler {
private:
  std::mutex m;
  std::list<Game *> games;
  int games_counter = 0;

  Game *get_game(const uint8_t &game_id);

public:
  GamesHandler();

  void add_game(Game *game);

  void delete_game(const int &game_id);

  Queue<std::shared_ptr<RunnableCommandGame>> *
  create_game(std::shared_ptr<Queue<GameState>> sender_queue, int *game_id,
              uint8_t &player_id);

  Queue<std::shared_ptr<RunnableCommandGame>> *
  join_game(std::shared_ptr<Queue<GameState>> sender_queue,
            const uint8_t &game_id, uint8_t &player_id);

  void start_game(const uint8_t &game_id, const uint8_t &player_id);

  bool game_exist(uint8_t game_id);

  void reap_dead();

  std::list<uint8_t> *obtain_all_games_id();

  ~GamesHandler();

  GamesHandler(const GamesHandler &) = delete;
  GamesHandler &operator=(const GamesHandler &) = delete;
};

#endif
