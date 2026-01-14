#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <algorithm>
#include <list>
#include <vector>
#include <mutex>

#include "../runnable_commands/command_runnable_game.h"
#include "server_game.h"

class Command;

class GamesHandler {
private:
  std::mutex m;
  std::list<Game *> games;
  int games_counter = 0;
  std::map<uint8_t, std::string> worlds_map;
  WorldsReader worlds_reader;
  //std::map<int, World> worlds;
  //std::vector<std::string> world_names;
  GameConfig games_config = GameConfig(std::string(RESOURCES_PATH) + "/game_config.yml");

  public:
  GamesHandler();

  Game *get_game(const uint8_t &game_id);

  void add_game(Game *game);

  void delete_game(const uint8_t &game_id);

  Game *
  create_game(std::shared_ptr<Queue<GameState>> sender_queue, uint8_t &player_id, uint8_t &world_id);

  Game *
  join_game(std::shared_ptr<Queue<GameState>> sender_queue, const uint8_t &game_id, uint8_t &player_id);

  void start_game(const uint8_t &game_id, const uint8_t &player_id);

  //World select_world(int world_id, const uint8_t& game_id);

  //World get_game_world(const uint8_t& game_id);

  bool game_exist(uint8_t game_id);

  void reap_dead();

  std::list<uint8_t> *obtain_all_games_id();

  std::map<uint8_t, std::string> get_worlds_map() const;

  ~GamesHandler();

  GamesHandler(const GamesHandler &) = delete;
  GamesHandler &operator=(const GamesHandler &) = delete;
};

#endif
