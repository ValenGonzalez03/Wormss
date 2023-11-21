#ifndef RUNNABLE_STOP_MOVING_H_
#define RUNNABLE_STOP_MOVING_H_

#include "command_runnable_game.h"
#include "../common/stop_moving.h"

class RunnableStopMoving : public RunnableCommandGame {

public:

    RunnableStopMoving(int clt_id, Socket &skt, bool *was_closed) : RunnableCommandGame(std::make_shared<StopMoving>(clt_id, skt, was_closed)) {}

    void run(GameManager &game_manager) override {
        game_manager.stop_moving();
    }

};

#endif
