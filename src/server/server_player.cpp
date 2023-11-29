#include "server_player.h"

#include <utility>
#include <vector>

Player::Player(Socket &&peer, GamesHandler &games_handler,
               std::shared_ptr<Queue<GameState>> sender_queue,
               uint8_t player_id)
    : skt(std::move(peer)), sender_queue(sender_queue),
      games_handler(games_handler), protocol(std::move(skt)),
      sender(protocol, sender_queue, keep_playing), player_id(player_id),
      client_handler(skt, protocol, games_handler, sender, sender_queue,
                     keep_playing, in_game, player_id) {}

void Player::start() {
  client_handler.start();
  //sender.start();
}

void Player::kill() {
  if (not is_dead()) {
    keep_playing = false;
    protocol.close_socket();
  }
}

void Player::join() {
  client_handler.join_sender();
  client_handler.join();
}

bool Player::is_dead() { return not keep_playing; }
