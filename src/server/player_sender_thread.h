#ifndef PLAYER_SENDER_THREAD_H
#define PLAYER_SENDER_THREAD_H

#include "../common/thread.h"
#include "../common/socket.h"
#include "../common/queue.h"
#include "server_protocol.h"
#include "game_state.h"

class PlayerSender : public Thread {
	private:
	Socket& skt;
	ServerProtocol& protocol;
	Queue<GameState*>* sender_queue;
	std::atomic<bool>& keep_playing;

	public:
	/*
	 * Constructor de la clase.
	 * */
	explicit PlayerSender(Socket& skt, ServerProtocol& protocol, Queue<GameState*>* sender_queue, 
					std::atomic<bool>& keep_playing);

	/*
	 * Ejecuta el loop del hilo Sender.
	 * */
	virtual void run() override;
	
	~PlayerSender();
		
	PlayerSender(const PlayerSender&) = delete;
	PlayerSender& operator=(const PlayerSender&) = delete;
};

#endif
