#include "server_player.h"

#include <utility>
#include <vector>

Player::Player(Socket peer):
        skt(std::move(peer)),
        client_handler(skt, keep_playing, in_game) 
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
