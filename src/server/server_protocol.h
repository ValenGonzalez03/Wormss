#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <vector>
#include "../common/socket.h"
#include "commands/command.h"
#include "game_state.h"
#include <memory>

class ServerProtocol {
    Socket& peer;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
    explicit ServerProtocol(Socket& peer);

    //std::unique_ptr<Command> receive_command(bool& was_closed);   VER ESTO

    int send(GameState& game_state, bool& was_closed);
};

#endif
