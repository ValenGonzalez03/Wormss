#include "client_handler_thread.h"

ClientHandler::ClientHandler(Socket& skt, ServerProtocol& protocol, GamesHandler& games_handler, Game& game, PlayerSender& sender, std::atomic<bool>& keep_playing, std::atomic<bool>& in_game):
        skt(skt), protocol(protocol), games_handler(games_handler), game(game), keep_playing(keep_playing), in_game(in_game), sender(sender) {}

void ClientHandler::run() {
	bool was_closed = false;
    try {
        while (keep_playing) {
            //protocol.receive...
            if(not in_game) {		// comunicacion sincronica
				std::vector<char> command = protocol.receive_command(was_closed);
				//games_handler.join/create
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
