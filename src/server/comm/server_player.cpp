#include "server_player.h"

#include <utility>
#include <vector>

Player::Player(uint8_t player_id, Game* game, GamesHandler& games_handler, PlayerSender& sender,
               std::shared_ptr<Queue<GameState>> sender_queue, ServerProtocol& protocol) :
    player_id(player_id), game(game), games_handler(games_handler), sender(sender), sender_queue(sender_queue),
    protocol(protocol) {}

void Player::initialize_game() {
  if (!game) {
    //Deberia lanzar una excepcion.
    std::cout << "No existe juego" << std::endl;
    return;
  }
  game->charge_world();
}

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

Queue<std::shared_ptr<RunnableCommandGame>>& Player::get_commands_queue_game() {
  if (!game) {
    throw std::runtime_error("No commands queue in game.");  // (?)
  }
  return game->get_commands_queue();
}

void Player::manage_create_game() {
  bool was_closed = false;
  protocol.send_byte(player_id, &was_closed);

  auto worlds_map = games_handler.get_worlds_map();
  sender.send_worlds_map(worlds_map);
  uint8_t world_id = protocol.recv_world_id(&was_closed);

  Game* game = games_handler.create_game(sender_queue, player_id, world_id);  // Asigno sender_queue al broadcaseter.
  this->game = game;
  has_game_assigned = true;

  //std::cout << "Cantidad Worms en juego: " << this->game->get_world().get_worms().size() << std::endl;

  sender.send_id(game->get_game_id());  // Ahora mando el game_id despues (Para tener en cuenta en el cliente)
  sender.send_world(game->get_world());
  std::cout << "Client of id: " << (int)player_id << " created game id: " << int(game->get_game_id()) << std::endl;
}

void Player::manage_join_game(uint8_t game_id) {
  bool was_closed = false;
  protocol.send_byte(player_id, &was_closed);

  Game* game = games_handler.join_game(sender_queue, game_id, player_id);
  std::cout << "Client of id: " << (int)player_id << " joined game id: " << int(game_id) << std::endl;
  sender.send_world(game->get_world());

  this->game = game;
  //in_game = true;
  //sender.start();
}

void Player::manage_start_game() {
  bool was_closed = false;
  initialize_game();
  sender.start();
  while (not protocol.recv_client_ready(&was_closed)) {
    ;
  }
  games_handler.start_game(get_game_id(), player_id);
  std::cout << "Client of id: " << (int)player_id << " started game of id: " << (int)get_game_id() << std::endl;
}