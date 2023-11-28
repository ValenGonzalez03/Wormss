#ifndef RUNNABLE_STOP_MOVING_H_
#define RUNNABLE_STOP_MOVING_H_

#include "../../common/commands/stop_moving.h"
#include "command_runnable_game.h"

class RunnableStopMoving : public RunnableCommandGame {

public:
  RunnableStopMoving(int clt_id, Socket &skt, bool *was_closed)
      : RunnableCommandGame(
            std::make_shared<StopMoving>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override { game_manager.stop_moving(); }
};

#endif
