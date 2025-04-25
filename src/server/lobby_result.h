#ifndef LOBBY_RESULT_H
#define LOBBY_RESULT_H

#include "../common/lib/queue.h"
#include "runnable_commands/command_runnable_game.h"

class LobbyResult {
private:
  const uint8_t player_id;
  const uint8_t game_id;
  Queue<std::shared_ptr<RunnableCommandGame>> *commands;
  std::vector<std::string> world_names;
  bool was_game_created = false;
  bool was_player_joined = false;
  bool was_game_started = false;

public:
  explicit LobbyResult(const uint8_t &player_id, const uint8_t &game_id,
                       Queue<std::shared_ptr<RunnableCommandGame>> *commands, std::vector<std::string> world_names)
      : player_id(player_id), game_id(game_id), commands(commands), world_names(world_names) {}

  Queue<std::shared_ptr<RunnableCommandGame>> *get_commands() {
    return commands;
  }

  void game_created() { was_game_created = true; }

  bool get_game_created() { return was_game_created; }

  void player_joined() { was_player_joined = true; }

  bool get_player_joined() { return was_player_joined; }

  void game_started() { was_game_started = true; }

  bool get_game_started() { return was_game_started; }

  uint8_t get_game_id() { return game_id; }

  std::vector<std::string>& get_worlds_names() { return world_names; }

  LobbyResult(const LobbyResult &) = delete;
  LobbyResult &operator=(const LobbyResult &) = delete;
};

#endif
