#ifndef RUNNABLE_JUMP_H_
#define RUNNABLE_JUMP_H_

#include "command_runnable_game.h"
#include "../common/jump.h"

class RunnableJump : public RunnableCommandGame {

public:

    explicit RunnableJump(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandGame(std::make_shared<Jump>(clt_id, skt, was_closed)) {}

    void run(GameManager &game_manager) override {
        game_manager.jump(command->get_client_id(), ((Jump*) command.get())->get_direction());
    }

};


#endif
