#include "server_player.h"

#include <utility>
#include <vector>
#include <string>

Player::Player(uint8_t player_id) : player_id(player_id) {}

Game* Player::create_game(uint8_t game_id, const World& world, const GameConfig& game_config,
                          PlayerSender& sender,Queue<game_command_ptr>& receiver_queue) {
  this->game = new Game(game_id, world, game_config, receiver_queue);
  game->add_player(sender, player_id);
  has_game_assigned = true;

  std::string str = "Client of id: " + std::to_string(static_cast<int>(player_id)) +
                    " created game id: " + std::to_string(static_cast<int>(game->get_game_id())) + "\n";
  std::cout << str;

  return game;
}

void Player::join_game(Game* game, PlayerSender& sender) {
  if (!game->is_started()) {
    game->add_player(sender, player_id);
    this->game = game;
    has_game_assigned = true;
    // Nuevamente, si la partida ya esta empezada habria que tirar una excepcion o avisar de alguna manera.
  }
  std::cout << "Client of id: " << static_cast<int>(player_id)
            << " joined game id: " << static_cast<int>(game->get_game_id()) << std::endl;
}

void Player::start_game() {
  if (!game->is_started()) {
    game->send_info_to_start_to_players();
    game->turn_to_started();
    game->start();
    game->charge_world();
  } else {
    std::cout << "No hago nada porque el juego ya empezó." << std::endl;
  }
}

World* Player::get_world() { return game->get_world(); }

uint8_t Player::get_game_id() const {
  if (!game) {
    return -1;
  }
  return game->get_game_id();
}

bool Player::has_game_started() {
  if (!has_game_assigned || !game) {
    return false;
  }
  return game->is_started();
}

bool Player::has_game_finished() {
  if (!has_game_assigned) {
    return false;
  } else if (!game) {
    return true;
  }
  return game->is_dead();
}

uint8_t Player::get_id() const { return player_id; }
