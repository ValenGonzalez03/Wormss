#include "player_sender_thread.h"

PlayerSender::PlayerSender(Protocol& protocol, std::shared_ptr<Queue<GameState>> sender_queue, std::atomic<bool>& keep_playing) :
        protocol(protocol), sender_queue(sender_queue), keep_playing(keep_playing) {}

void PlayerSender::run() {
    bool was_closed = false;
    try {
        while (keep_playing) {
			GameState game_state = sender_queue->pop();
			protocol.send_game_state(game_state);
        }
    } catch (const std::exception& err) {
	}
}

void PlayerSender::send_player_id(const int player_id) {
    protocol.send_player_id(player_id);
}

PlayerSender::~PlayerSender() {
    keep_playing = false;
}
