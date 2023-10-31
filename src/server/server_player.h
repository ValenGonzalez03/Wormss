#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "../common/socket.h"
#include <vector>
#include <atomic>

class Player {
	Socket skt;
	std::atomic<bool> keep_talking {true};
	//Sender sender;
	//Receiver receiver;
	
		
	public:
	/*
	 * Constructor de la clase.
	 * */
	explicit Player(Socket peer);
	
	/*
	 * Ejecuta los hilos Sender y Receiver.
	 * */
	void start();
	
	/*
	 * Si siguen vivos, cierra los hilos Sender y Reciever.
	 * */
	void kill();
	
	/*
	 * Joinea los hilos Sender y Receiver.
	 * */
	void join();
	
	/*
	 * Devuelve true si alguno de sus hilos esta muerto. False en caso contrario.
	 * */
	bool is_dead();
	
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
};

#endif
