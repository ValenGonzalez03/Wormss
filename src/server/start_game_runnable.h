#ifndef RUNNABLE_START_GAME_H_
#define RUNNABLE_START_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/start_game.h"

class RunnableStartGame : public RunnableCommandLobby {

public:

    RunnableStartGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<StartGame>(clt_id, skt, was_closed)) {}

    void run(/* Alguna clase de server para comandos de lobby*/) override {
        
    }

};

#endif
