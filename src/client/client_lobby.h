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

public:
  Lobby(ClientProtocol &prot) : prot(prot) {}

  void run_lobby() {
    char option_selected = '\0';
    do {
      std::cout << "Ingrese 'c' si quiere crear una partida o 'j' si quiere "
                   "unirse a una:"
                << std::endl;
      std::cin >> option_selected;
    } while (option_selected != 'c' && option_selected != 'j');

    if (option_selected == 'c') {
      // Elegir escenario
      //  bool was_closed = false;
      //  std::vector<std::string> world_names =
      //  prot.recv_worlds_names(&was_closed); std::cout << "Juego creado,
      //  esperando jugadores..." << std::endl;
      // Elijo un world
      //  prot.send_world_name_selected(world_name);
      CreateGame create_comm = CreateGame();
      prot.send_command(create_comm);
    } else if (option_selected == 'j') {
      std::cout << "Ingrese el codigo de la partida para unirse:" << std::endl;
      int game_id;
      std::cin >> game_id;
      JoinGame join_comm(game_id);
      prot.send_command(join_comm);
    }

    // Recibe el player_id
    int player_id = prot.receive_id();
    std::cout << "Tu player_id es: " << player_id << std::endl;
    // Si se crea una partida tambien se recibe el game_id
    int game_id;
    if (option_selected == 'c') {
      game_id = prot.receive_id();
      std::cout << "El game id es: " << game_id << std::endl;
    } else if (option_selected == 'j') {
      return;
    }

    char command_lobby = '\0';
    while (command_lobby != 's') {
      std::cout << "El creador de la partida cuando quiere empezarla debe "
                   "ingresar 's'"
                << std::endl;
      std::cin >> command_lobby;
    }
    StartGame start = StartGame(game_id);
    prot.send_command(start);
  }
};

#endif
