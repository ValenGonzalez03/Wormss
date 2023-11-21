#ifndef RUNNABLE_CREATE_GAME_H_
#define RUNNABLE_CREATE_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/create_game.h"

class RunnableCreateGame : public RunnableCommandLobby {

public:

    RunnableCreateGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<CreateGame>(clt_id, skt, was_closed)) {}

    void run(/* Alguna clase de server para comandos de lobby*/) override {
        
    }

};

#endif
