#ifndef COMMAND_H
#define COMMAND_H

#include "../server_games_handler.h"
#include "../game_manager.h"
#include "../../common/protocol_codes.h"

class Command {
    int player_id;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
    explicit Command(int player_id);

    virtual Queue<Command*>* handle_command(GamesHandler& games_handler, Queue<GameState*>* sender_queue) = 0;

    virtual void handle_command(GameManager& game_manager) = 0;
};

#endif
