#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <vector>
#include "../common/socket.h"
#include "commands/command.h"

class ServerProtocol {
    Socket& peer;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
    explicit ServerProtocol(Socket& peer);

    std::unique_ptr<Command> receive_command(bool& was_closed);

    int send(std::vector<char> command, bool& was_closed);
};

#endif
