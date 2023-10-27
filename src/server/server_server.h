#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <string>

class Server {
	const std::string& servname;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
	explicit Server(const std::string& servname);
	
	/*
	 * Comienza la ejecución del server.
	 * */
	void run();
	
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
};

#endif


