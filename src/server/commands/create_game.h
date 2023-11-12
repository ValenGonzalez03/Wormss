#ifndef CREATE_GAME_H
#define CREATE_GAME_H

#include "command.h"
#include "../server_games_handler.h"
#include "../../common/protocol_codes.h"

class CreateGame : public Command {
    int player_id;
	
	public:
	/*
	 * Constructor de la clase.
	 * */
    explicit CreateGame(int player_id);

    Queue<Command*>* handle_command(GamesHandler& games_handler, Queue<GameState*>* sender_queue) override {
        return games_handler.create_game(sender_queue, player_id);
    };
};

#endif
