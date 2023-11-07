#ifndef MOVE_H
#define MOVE_H

#include "command.h"
#include "../server_game.h"
#include "../../common/protocol_codes.h"

class Move : public Command {
    int player_id;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
    explicit Move(int player_id);

    void handle_command(Game& game) override {
        //game.move(id_client);
    };
};

#endif
