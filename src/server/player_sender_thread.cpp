#include "player_sender_thread.h"

PlayerSender::PlayerSender(Socket& skt, ServerProtocol& protocol, Queue<std::vector<char>>* sender_queue, std::atomic<bool>& keep_playing) :
        skt(skt), protocol(protocol), sender_queue(sender_queue), keep_playing(keep_playing) {}

void PlayerSender::run() {
    bool was_closed = false;
    try {
        while (keep_playing) {
			std::vector<char> command = sender_queue->pop();
			protocol.send(command, was_closed);
            
        }
    } catch (const std::exception& err) {}
}

PlayerSender::~PlayerSender() {
    keep_playing = false;
}
