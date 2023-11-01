#ifndef CLIENT_HANDLER_THREAD_H
#define CLIENT_HANDLER_THREAD_H

#include "../common/thread.h"
#include "../common/socket.h"
#include "player_sender_thread.h"
//#include "server_protocol.h"

class ClientHandler : public Thread {
	private:
	Socket& skt;
	//ServerProtocol protocol;
	std::atomic<bool>& keep_playing;
	std::atomic<bool>& in_game;
	PlayerSender sender;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
	explicit ClientHandler(Socket& skt, 
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
