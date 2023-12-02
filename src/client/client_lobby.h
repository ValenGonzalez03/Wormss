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
  explicit Lobby(ClientProtocol &prot) : prot(prot), player_id(-1) {}

  void print_menu() {
    std::cout << "Ingrese 'c' si quiere crear una partida o 'j' si quiere unirse a una:" << std::endl;
  }

  char get_option() {
    char option;
    std::cin >> option;
    return option;
  }

  int get_world_id(std::vector<std::string>& world_names) {
    int option;
    while (true){
      std::cin >> option;
      if (option < world_names.size()){
        return option;
      }
      std::cout << "Elija una opción válida" << std::endl;
    }
  }

  void create_game(ClientProtocol& prot, int& player_id, bool* was_closed) {
    CreateGame create_comm = CreateGame();
    prot.send_command(create_comm);

    player_id = prot.receive_id();
    std::cout << "Tu player_id es: " << player_id << std::endl;

    int game_id = prot.receive_id();
    std::cout << "El game id es: " << game_id << std::endl;

    std::vector<std::string> world_names = prot.recv_worlds_names(was_closed);
    show_worlds(world_names);

    int world_id = get_world_id(world_names);

    prot.send_world_id(world_id, was_closed);

    wait_start_command(prot, game_id);
  }

  void join_game(ClientProtocol& prot, int& player_id, bool *was_closed) {
    std::cout << "Ingrese el codigo de la partida para unirse:" << std::endl;
    int game_id = 0;
    std::cin >> game_id;

    JoinGame join_comm(game_id);
    prot.send_command(join_comm);

    player_id = prot.receive_id();
    std::cout << "Tu player_id es: " << player_id << std::endl;
  }

  void show_worlds(const std::vector<std::string>& world_names) {
    std::cout << "Elige un numero de escenario:" << std::endl;
    for (int i = 0; i < world_names.size(); ++i) {
        std::cout << i << ": " << world_names[i] << std::endl;
    }
  }

  void wait_start_command(ClientProtocol& prot, int game_id) {
    char command_lobby = '\0';
    while (command_lobby != 's') {
        std::cout << "El creador de la partida cuando quiere empezarla debe ingresar 's'" << std::endl;
        std::cin >> command_lobby;
    }

    StartGame start(game_id);
    prot.send_command(start);
  }

  void run_lobby() {
    char option_selected = '\0';
    do {
        print_menu();
        option_selected = get_option();
    } while (option_selected != 'c' && option_selected != 'j');

    bool was_closed = false;
    int player_id = 0;

    if (option_selected == 'c') {
        create_game(prot, player_id, &was_closed);
    } else if (option_selected == 'j') {
        join_game(prot, player_id, &was_closed);
    }
  }

  uint8_t get_player_id() { return player_id; }

};

#endif

/*
    char option_selected = '\0';
    do {
      std::cout << "Ingrese 'c' si quiere crear una partida o 'j' si quiere "
                   "unirse a una:"
                << std::endl;
      std::cin >> option_selected;
    } while (option_selected != 'c' && option_selected != 'j');

    bool was_closed = false;

    if (option_selected == 'c') {
      CreateGame create_comm = CreateGame();
      prot.send_command(create_comm);
      

    } else if (option_selected == 'j') {
      std::cout << "Ingrese el codigo de la partida para unirse:" << std::endl;
      int game_id = 0;
      std::cin >> game_id;
      JoinGame join_comm(game_id);
      prot.send_command(join_comm);
      // Recibo el world en el cliente
    }

    // Recibe el player_id
    player_id = prot.receive_id();
    std::cout << "Tu player_id es: " << player_id << std::endl;
    // Si se crea una partida tambien se recibe el game_id
    int game_id = 0;

    if (option_selected == 'c') {
      game_id = prot.receive_id();
      std::cout << "El game id es: " << game_id << std::endl;
      std::vector<std::string> world_names = prot.recv_worlds_names(&was_closed);
      std::cout << "Elige un numero de escenario: " << game_id << std::endl;
      int i = 0;
      for (auto &world_name : world_names) {
        std::cout << i << ": " << world_name << std::endl;
        i++;
      }

      int world_id;
      std::cin >> world_id;

      prot.send_world_id(world_id, &was_closed);
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
*/
