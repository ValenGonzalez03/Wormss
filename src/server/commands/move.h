#ifndef MOVE_H
#define MOVE_H

#include "command.h"
#include "../game_manager.h"
#include "../../common/protocol_codes.h"

class Move : public Command {
    int player_id;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
    explicit Move(int player_id);

    void handle_command(GameManager& game_manager) override {
        game_manager.move(player_id, 0);
    };
};

#endif
