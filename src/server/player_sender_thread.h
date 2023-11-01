#ifndef PLAYER_SENDER_THREAD_H
#define PLAYER_SENDER_THREAD_H

#include "../common/thread.h"
#include "../common/socket.h"
//#include "server_protocol.h"

class PlayerSender : public Thread {
	private:
	Socket& skt;
	//ServerProtocol protocol;
	std::atomic<bool>& keep_playing;
	//Queue<Message>& sender_queue;

	public:
	/*
	 * Constructor de la clase.
	 * */
	explicit PlayerSender(Socket& skt, 
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

