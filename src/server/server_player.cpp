#include "server_player.h"

#include <utility>
#include <vector>

#define QUEUE_MAX_SIZE 20

Player::Player(Socket peer, GamesHandler& games_handler):
        skt(std::move(peer)),
        sender_queue(QUEUE_MAX_SIZE),
        games_handler(games_handler),
        protocol(skt),
        sender(skt, protocol, &sender_queue, keep_playing),
        client_handler(skt, protocol, games_handler, sender, &sender_queue, keep_playing, in_game)
        {}

void Player::start() {
	client_handler.start();
}

void Player::kill() {
    if (not is_dead()) {
		keep_playing = false;
        skt.shutdown(2);
        skt.close();
    }
}

void Player::join() {
	client_handler.join_sender();
    client_handler.join();
}

bool Player::is_dead() { 
	return not keep_playing;
}
