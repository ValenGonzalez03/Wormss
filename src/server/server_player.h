#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "../common/socket.h"
#include <vector>
#include <atomic>

#include "client_handler_thread.h"
#include "player_sender_thread.h"

class Player {
	private:
	Socket skt;
	std::atomic<bool> keep_playing {true};
	std::atomic<bool> in_game {false};
	//PlayerSender sender;
	ClientHandler client_handler;
		
	public:
	/*
	 * Constructor de la clase.
	 * */
	explicit Player(Socket peer);
	
	/*
	 * Ejecuta los hilos.
	 * */
	void start();
	
	/*
	 * Si siguen vivos, cierra los hilos.
	 * */
	void kill();
	
	/*
	 * Joinea los hilos.
	 * */
	void join();
	
	/*
	 * Devuelve true si sus hilos estan muertos. False en caso contrario.
	 * */
	bool is_dead();
	
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
};

#endif
