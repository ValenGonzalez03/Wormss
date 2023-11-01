#include "player_sender_thread.h"

PlayerSender::PlayerSender(Socket& skt, std::atomic<bool>& keep_playing):
        skt(skt), keep_playing(keep_playing) {}

void PlayerSender::run() {
    try {
        while (keep_playing) {
			//sender_queue.pop()
			//protocol.send...
            
        }
    } catch (const std::exception& err) {}
}

PlayerSender::~PlayerSender() {
    keep_playing = false;
}
