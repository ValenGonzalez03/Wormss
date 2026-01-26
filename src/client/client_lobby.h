#ifndef CLIENT_LOBBY_H
#define CLIENT_LOBBY_H

#include "../common/commands/create_game.h"
#include "../common/commands/join_game.h"
#include "../common/commands/start_game.h"
#include "comm/client_protocol.h"
#include <iostream>

class Lobby {
 private:
  ClientProtocol& prot;
  uint8_t player_id;

 public:
  explicit Lobby(ClientProtocol& prot);

  void run_lobby();

  void print_menu();

  char get_option();

  void create_game(ClientProtocol& prot, uint8_t& player_id, bool* was_closed);

  void show_worlds(const std::map<uint8_t, std::string>& world_names);

  uint8_t select_world(std::map<uint8_t, std::string>& worlds_map);

  void join_game(ClientProtocol& prot, uint8_t& player_id, bool* was_closed);

  void wait_start_command(ClientProtocol& prot, int game_id);

  uint8_t get_player_id();
};

#endif