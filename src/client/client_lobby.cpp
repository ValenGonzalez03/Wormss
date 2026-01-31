#include "client_lobby.h"

Lobby::Lobby(ClientProtocol& prot) : prot(prot), player_id(-1) {}

void Lobby::print_menu() {
  std::cout << "Ingrese 'c' si quiere crear una partida o 'j' si quiere unirse a una:" << std::endl;
}

char Lobby::get_option() {
  char option;
  std::cin >> option;
  return option;
}

uint8_t Lobby::select_world(const std::map<uint8_t, std::string>& worlds_map) {
  int option;
  while (true) {
    std::cin >> option;
    if (worlds_map.find(static_cast<uint8_t>(option)) != worlds_map.end()) {
      return option;
    }
    std::cout << "Elija una opción válida" << std::endl;
  }
}

void Lobby::create_game(ClientProtocol& prot, bool* was_closed) {
  CreateGame create_comm = CreateGame();
  prot.send_command(create_comm);

  this->player_id = prot.recv_byte(was_closed);
  std::cout << "Tu player_id es: " << std::to_string(player_id) << std::endl;

  std::map<uint8_t, std::string> worlds_map = prot.recv_worlds_map(was_closed);
  show_worlds(worlds_map);

  uint8_t world_id = select_world(worlds_map);
  std::cout << "WORLD_ID SELECCIONADO: " << static_cast<int>(world_id) << std::endl;

  prot.send_world_id(world_id, was_closed);

  int game_id = prot.recv_byte(was_closed);
  std::cout << "GAME ID SELECCIONADO: " << game_id << std::endl;

  send_start_game(prot, game_id);
}

void Lobby::join_game(ClientProtocol& prot, bool* was_closed) {
  std::cout << "Ingrese el codigo de la partida para unirse:" << std::endl;
  int game_id = 0;
  std::cin >> game_id;

  JoinGame join_comm(game_id);
  prot.send_command(join_comm);

  this->player_id = prot.recv_byte(was_closed);
  std::cout << "Tu player_id es: " << std::to_string(player_id) << std::endl;

  wait_start_game(prot);
}

void Lobby::show_worlds(const std::map<uint8_t, std::string>& worlds_map) {
  std::cout << "Elige un numero de escenario:" << std::endl;
  for (const auto& world_pair : worlds_map) {
    std::cout << static_cast<int>(world_pair.first) << ": " << world_pair.second << std::endl;
  }
}

void Lobby::send_start_game(ClientProtocol& prot, int game_id) {
  char command_lobby = '\0';
  while (command_lobby != 's') {
    std::cout << "El creador de la partida cuando quiere empezarla debe ingresar 's'" << std::endl;
    std::cin >> command_lobby;
  }
  StartGame start(game_id);
  prot.send_command(start);
}

void Lobby::wait_start_game(ClientProtocol& prot) {}

void Lobby::run_lobby() {
  char option_selected = '\0';
  do {
    print_menu();
    option_selected = get_option();
  } while (option_selected != 'c' && option_selected != 'j');

  bool was_closed = false;

  if (option_selected == 'c') {
    create_game(prot, &was_closed);
  } else if (option_selected == 'j') {
    join_game(prot, &was_closed);
  }
}

uint8_t Lobby::get_player_id() { return player_id; }
