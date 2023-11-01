#include "client_handler_thread.h"

ClientHandler::ClientHandler(Socket& skt, std::atomic<bool>& keep_playing, std::atomic<bool>& in_game):
        skt(skt), keep_playing(keep_playing), in_game(in_game), sender(skt, keep_playing)  {}

void ClientHandler::run() {
    try {
		
        while (keep_playing) {
            //protocol.receive...
            if(not in_game) {		// comunicacion sincronica
				//protocol.receive
				//game_handler.join/create
				in_game = true;
				sender.start();
			} else {			   // comunicacion asincronica
				// todo lo del estado del juego
				//protocol.receive
				//game.realize_action
			}
        }
        
    } catch (const std::exception& err) {
    }
}

void ClientHandler::join_sender() {
	sender.join();
}

ClientHandler::~ClientHandler() {
	keep_playing = false;
}
