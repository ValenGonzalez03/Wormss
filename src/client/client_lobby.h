#ifndef CLIENT_LOBBY_H
#define CLIENT_LOBBY_H

#include "../common/commands/create_game.h"
#include "../common/commands/join_game.h"
#include "../common/commands/start_game.h"
#include "client_protocol.h"
#include <iostream>

class Lobby {
private:
  ClientProtocol &prot;
  uint8_t player_id;

public:
  explicit Lobby(ClientProtocol &prot);

  void print_menu();

  char get_option();

  int get_world_id(std::vector<std::string>& world_names);

  void create_game(ClientProtocol& prot, int& player_id, bool* was_closed);

  void join_game(ClientProtocol& prot, int& player_id, bool *was_closed);

  void show_worlds(const std::vector<std::string>& world_names);

  void wait_start_command(ClientProtocol& prot, int game_id);

  void run_lobby();

  uint8_t get_player_id();

};

#endif