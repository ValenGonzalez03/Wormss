#ifndef SERVER_ACEPTADOR_H
#define SERVER_ACEPTADOR_H

#include <list>
#include "player.h"
#include "../common/socket.h"
#include "../common/thread.h"

class Aceptador : public Thread {
	private:
	Socket skt;
	std::atomic<bool> is_alive {true};
	std::list<Player*> players;
	
	/*
	 * Busca todos los players muertos en la lista de players y los joinea.
	 */
	void reap_dead();
	
	/*
	 * Recorre la lista de players y mata todos los players aun vivos.
	 */
	void kill_all();
	
	public:
	/*
	 * Constructor de la clase.
	 * */
	explicit Aceptador(Socket& skt);
	
	/*
	 * Ejecuta el hilo Aceptador.
	 * */
	void run() override;
	
	/*
	 * Cierra el socket y detiene la espera de nuevos clientes.
	 */
	void kill();

	Aceptador(const Aceptador&) = delete;
	Aceptador& operator=(const Aceptador&) = delete;
};

#endif
