#include "client_handler_thread.h"

ClientHandler::ClientHandler(Socket& skt, Protocol& protocol, GamesHandler& games_handler, PlayerSender& sender, std::shared_ptr<Queue<GameState>> sender_queue, std::atomic<bool>& keep_playing, std::atomic<bool>& in_game):
        skt(skt), protocol(protocol), games_handler(games_handler), sender(sender), sender_queue(sender_queue), keep_playing(keep_playing), in_game(in_game) {}

void ClientHandler::run() {
	bool was_closed = false;
	//int player_id = 0;
	//std::shared_ptr<Command> command = std::make_shared<StartMoving>(0); SOLO PARA PRUEBAS
    try {
        while (keep_playing) {            
            if(not in_game) {		// comunicacion sincronica	
				//std::list<int>* games_id = games_handler.obtain_all_games_id();
				//protocol.send_games_id();
				int player_id;
				std::shared_ptr<Command> command = protocol.process_command();
								
				if(command->is_create_command()) {
					receiver_queue = games_handler.create_game(sender_queue, player_id);
				} else if(command->is_join_command()) {
					receiver_queue = games_handler.join_game(sender_queue, player_id, command->get_game_id());
				} else {
					continue;
				}
				in_game = true;
				sender.send_player_id(player_id);
				sender.start();
				
				/*
				receiver_queue = games_handler.create_game(sender_queue, 0);
				sender.start();
				in_game = true;
				*/
			} else {			   // comunicacion asincronica
				std::shared_ptr<Command> command = protocol.process_command();
				receiver_queue->push(command);
			}
        }

    } catch (const std::exception& err) {
		keep_playing = false;
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
