#ifndef SERVER_GAMES_HANDLER_H
#define SERVER_GAMES_HANDLER_H

#include <algorithm>
#include <list>
#include <vector>
#include <mutex>
#include <map>
#include <memory>
#include <string>

#include "../runnable_commands/command_runnable_game.h"
#include "server_game.h"
#include "game_manager.h"
#include "../comm/server_player.h"

class Command;

class GamesHandler {
 private:
  std::mutex m;
  std::list<Game *> games;
  int games_counter = 0;
  std::map<uint8_t, std::string> worlds_map;
  WorldsReader worlds_reader;
  // std::map<int, World> worlds;
  // std::vector<std::string> world_names;
  GameConfig games_config = GameConfig(std::string(RESOURCES_PATH) + "/game_config.yml");

 public:
  GamesHandler();

  Game *create_game(Player &player, const uint8_t &world_id,  // NOLINT(runtime/references)
                    PlayerSender &sender,                     // NOLINT(runtime/references)
                    ServerReceiver &receiver);                // NOLINT(runtime/references)

  Game *join_game(Player &player, const uint8_t &game_id,  // NOLINT(runtime/references)
                  PlayerSender &sender,                    // NOLINT(runtime/references)
                  ServerReceiver &receiver);               // NOLINT(runtime/references)

  Game *start_game(Player &player, const uint8_t &game_id);  // NOLINT(runtime/references)

  void delete_game(const uint8_t &game_id);

  Game *get_game(const uint8_t &game_id) const;

  bool game_exist(uint8_t game_id);

  void reap_dead();

  std::map<uint8_t, std::string> get_worlds_map() const;

  ~GamesHandler();

  GamesHandler(const GamesHandler &) = delete;
  GamesHandler &operator=(const GamesHandler &) = delete;
};

#endif
