#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <algorithm>
#include <list>
#include <vector>
#include <mutex>

#include "runnable_commands/command_runnable_game.h"
#include "server_game.h"
#include "worlds_reader.h"

class Command;

class GamesHandler {
private:
  std::mutex m;
  std::list<Game *> games;
  int games_counter = 0;
  std::vector<std::shared_ptr<World>> worlds;
  std::vector<std::string> world_names;
  GameConfig games_config = GameConfig(std::string(RESOURCES_PATH) + "/game_config.yml");

  public:
  GamesHandler();

  Game *get_game(const uint8_t &game_id);

  void add_game(Game *game);

  void delete_game(const uint8_t &game_id);

  Queue<std::shared_ptr<RunnableCommandGame>> *
  create_game(std::shared_ptr<Queue<GameState>> sender_queue, uint8_t& game_id,
              uint8_t &player_id, std::vector<std::string>& names);

  Queue<std::shared_ptr<RunnableCommandGame>> *
  join_game(std::shared_ptr<Queue<GameState>> sender_queue,
            const uint8_t &game_id, uint8_t &player_id);

  void start_game(const uint8_t &game_id, const uint8_t &player_id);

  World select_world(int world_id, const uint8_t& game_id);

  World& select_world(const uint8_t& game_id);

  bool game_exist(uint8_t game_id);

  void reap_dead();

  std::list<uint8_t> *obtain_all_games_id();

  ~GamesHandler();

  GamesHandler(const GamesHandler &) = delete;
  GamesHandler &operator=(const GamesHandler &) = delete;
};

#endif
