#ifndef COMMAND_H
#define COMMAND_H

#include "../server_game.h"
#include "../../common/protocol_codes.h"

class Command {
    int player_id;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
    explicit Command(int player_id);

    virtual void handle_command(Game& game) = 0;
};

#endif
