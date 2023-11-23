#include "client_handler_thread.h"
#include "command_runnable_lobby.h"
#include "command_runnable_game.h"

ClientHandler::ClientHandler(Socket& skt, ServerProtocol& protocol, GamesHandler& games_handler, PlayerSender& sender, std::shared_ptr<Queue<GameState>> sender_queue, std::atomic<bool>& keep_playing, std::atomic<bool>& in_game):
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
				int game_id;
				int player_id;
				
				std::shared_ptr<RunnableCommandLobby> runnable_command = protocol.process_command_lobby();
				//runnable_command->receive();
				
				//runnable_command->run(games_handler, sender, sender_queue, game_id, player_id);
				runnable_command->run(*this);
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

void ClientHandler::create_game() {
	uint8_t game_id;
	int player_id;
	receiver_queue = games_handler.create_game(sender_queue, game_id, player_id);
	sender.send_id(game_id);
	sender.send_id(player_id);
}

void ClientHandler::join_game(const uint8_t& game_id) {
	int player_id;
	receiver_queue = games_handler.join_game(sender_queue, game_id, player_id);
	sender.send_id(player_id);
}

void ClientHandler::join_sender() {
	if (in_game) {
		sender.join();
	}
}

ClientHandler::~ClientHandler() {
	keep_playing = false;
}
