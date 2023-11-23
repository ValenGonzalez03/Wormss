#ifndef RUNNABLE_JOIN_GAME_H_
#define RUNNABLE_JOIN_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/join_game.h"

class RunnableJoinGame : public RunnableCommandLobby {

public:
    RunnableJoinGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<JoinGame>(clt_id, skt, was_closed)) {}

    void run(ClientHandler& client_handler) override {
        client_handler.join_game(get_game_id());
    }

    uint8_t get_game_id() {
		return ((JoinGame*)command.get())->get_game_id();
	}
};

#endif
