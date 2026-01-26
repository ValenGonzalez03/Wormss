#ifndef RUNNABLE_STOP_GAME_H_
#define RUNNABLE_STOP_GAME_H_

#include "../../common/commands/stop_game.h"
#include "command_runnable_game.h"

class RunnableStopGame : public RunnableCommandGame {

 public:
  RunnableStopGame(uint8_t clt_id, Socket &skt, bool *was_closed) :
      RunnableCommandGame(std::make_shared<StopGame>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override { game_manager.set_game_finished(true); }
};

#endif