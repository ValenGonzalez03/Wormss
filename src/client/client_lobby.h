#ifndef CLIENT_LOBBY_H
#define CLIENT_LOBBY_H

#include "../common/commands/create_game.h"
#include "../common/commands/join_game.h"
#include "../common/commands/start_game.h"
#include "comm/client_protocol.h"
#include <iostream>
#include <map>
#include <string>

class Lobby {
 private:
  ClientProtocol& prot;
  uint8_t player_id;

 public:
  explicit Lobby(ClientProtocol& prot);  // NOLINT(runtime/references)

  void run_lobby();

  void print_menu();

  char get_option();

  void create_game(ClientProtocol& prot, uint8_t& player_id, bool* was_closed);  // NOLINT(runtime/references)

  void show_worlds(const std::map<uint8_t, std::string>& world_names);

  uint8_t select_world(const std::map<uint8_t, std::string>& worlds_map);

  void join_game(ClientProtocol& prot, uint8_t& player_id, bool* was_closed);  // NOLINT(runtime/references)

  void wait_start_command(ClientProtocol& prot, int game_id);  // NOLINT(runtime/references)

  uint8_t get_player_id();
};

#endif
