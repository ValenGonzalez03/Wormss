#include "client_handler_thread.h"
#include "command_runnable_lobby.h"
#include "command_runnable_game.h"

ClientHandler::ClientHandler(Socket& skt, ServerProtocol& protocol, GamesHandler& games_handler, PlayerSender& sender, std::shared_ptr<Queue<GameState>> sender_queue, std::atomic<bool>& keep_playing, std::atomic<bool>& in_game):
        skt(skt), protocol(protocol), games_handler(games_handler), sender(sender), sender_queue(sender_queue), keep_playing(keep_playing), in_game(in_game) {}

void ClientHandler::run() {
	bool was_closed = false;
	//std::shared_ptr<Command> command = std::make_shared<StartMoving>(0); SOLO PARA PRUEBAS
    try {
        while (keep_playing) {            
            if(not in_game) {		// comunicacion sincronica	
				//std::list<int>* games_id = games_handler.obtain_all_games_id();
				//protocol.send_games_id();
				
				std::shared_ptr<RunnableCommandLobby> runnable_command = protocol.process_command_lobby();
				
				runnable_command->run();
				
				
				receiver_queue = games_handler.create_game(sender_queue, 0);
				sender.start();
				in_game = true;
				
				
			} else {			   // comunicacion asincronica
				std::shared_ptr<RunnableCommandGame> runnable_command = protocol.process_command();
				receiver_queue->push(runnable_command);
			}
        }

    } catch (const LibError& libError) { // Si se cierra el skt
		keep_playing = false;
		//std::cerr << "LibError: " << libError.what() << std::endl;
    } catch (const std::runtime_error& runtimeError) { // Si se procesa mal un cmd
		keep_playing = false;
		std::cerr << "RuntimeError: " << runtimeError.what() << std::endl;
	}
}

void ClientHandler::create_game(Command *command) {
	receiver_queue = games_handler.create_game(sender_queue, command->get_client_id());
	in_game = true;
	sender.start();
}

void ClientHandler::join_game(Command *command) {
	receiver_queue = games_handler.join_game(sender_queue, command->get_client_id(), command->get_game_id());
	in_game = true;
	sender.start();
}

void ClientHandler::join_sender() {
	if (in_game) {
		sender.join();
	}
}

ClientHandler::~ClientHandler() {
	keep_playing = false;
}
