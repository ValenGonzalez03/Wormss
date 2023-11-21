#ifndef RUNNABLE_JOIN_GAME_H_
#define RUNNABLE_JOIN_GAME_H_

#include "command_runnable_lobby.h"
#include "../common/join_game.h"

class RunnableJoinGame : public RunnableCommandLobby {

public:

    RunnableJoinGame(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandLobby(std::make_shared<JoinGame>(clt_id, skt, was_closed)) {}

    void run(/* Alguna clase de server para comandos de lobby*/) override {
        
    }

};

#endif
