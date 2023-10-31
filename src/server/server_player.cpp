#include "server_player.h"

#include <utility>
#include <vector>

//#include "player_receiver.h"
//#include "player_sender.h"

Player::Player(Socket peer):
        skt(std::move(peer))
        //sender(skt, keep_talking),
        //receiver(skt, keep_talking) 
        {}

void Player::start() {

    //sender.start();
    //receiver.start();
}

void Player::kill() {
    if (not is_dead()) {
        skt.shutdown(2);
        skt.close();
    }
}

void Player::join() {
    //sender.join();
    //receiver.join();
}

bool Player::is_dead() { 
	return true;
	//return (sender.is_dead() || receiver.is_dead()); 
	}
