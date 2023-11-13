#include "client_handler_thread.h"

ClientHandler::ClientHandler(Socket& skt, ServerProtocol& protocol, GamesHandler& games_handler, PlayerSender& sender, Queue<GameState>* sender_queue, std::atomic<bool>& keep_playing, std::atomic<bool>& in_game):
        skt(skt), protocol(protocol), games_handler(games_handler), sender(sender), sender_queue(sender_queue), keep_playing(keep_playing), in_game(in_game) {}

void ClientHandler::run() {
	bool was_closed = false;
    try {
        while (keep_playing) {
            //protocol.receive...
            if(not in_game) {		// comunicacion sincronica
				//std::unique_ptr<Command> command = protocol.receive_command(was_closed);
				//Queue<Command*>* game_commands_queue = command->handle_command(games_handler, sender_queue);
				
				//receiver_queue = games_handler.create_game(sender_queue, 0);
				in_game = true;
				sender.start();
			} else {			   // comunicacion asincronica
				// todo lo del estado del juego
				//protocol.receive
				//receiver_queue->push(command);
			}
        }

    } catch (const std::exception& err) {
    }
}

void ClientHandler::join_sender() {
	if (in_game) {
		sender.join();
	}
}

ClientHandler::~ClientHandler() {
	keep_playing = false;
}
