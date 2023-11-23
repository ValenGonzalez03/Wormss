#ifndef CLIENT_HANDLER_THREAD_H
#define CLIENT_HANDLER_THREAD_H

#include "../common/thread.h"
#include "../common/socket.h"
#include "player_sender_thread.h"
#include "server_games_handler.h"
#include "lobby_result.h"
#include "command_runnable_lobby.h"
#include "command_runnable_game.h"

class ClientHandler : public Thread {
	private:
	Socket& skt;
	ServerProtocol& protocol;
	GamesHandler& games_handler;
	PlayerSender& sender;
	Queue<std::shared_ptr<RunnableCommandGame>>* game_commands;
	std::shared_ptr<Queue<GameState>> sender_queue;
	std::atomic<bool>& keep_playing;
	std::atomic<bool>& in_game;
	std::unique_ptr<LobbyResult> lobby_result;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
	explicit ClientHandler(Socket& skt, ServerProtocol& protocol, GamesHandler& games_handler, PlayerSender& sender, std::shared_ptr<Queue<GameState>> sender_queue,
					  std::atomic<bool>& keep_playing, std::atomic<bool>& in_game);
	
	/*
	 * Ejecuta el loop del ClientHandler.
	 * */
	virtual void run() override;
	
	/*
	 * Joinea el hilo Sender.
	 */
	void join_sender();
		
	~ClientHandler();
	
	ClientHandler(const ClientHandler&) = delete;
	ClientHandler& operator=(const ClientHandler&) = delete;
};

#endif
