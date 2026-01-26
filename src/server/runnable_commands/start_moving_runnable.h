#ifndef RUNNABLE_START_MOVING_H_
#define RUNNABLE_START_MOVING_H_

#include "../../common/commands/start_moving.h"
#include "command_runnable_game.h"

class RunnableStartMoving : public RunnableCommandGame {

 public:
  explicit RunnableStartMoving(uint8_t clt_id, Socket &skt, bool *was_closed) :
      RunnableCommandGame(std::make_shared<StartMoving>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override {
    game_manager.move(command->get_client_id(), ((StartMoving *)command.get())->get_direction());
  }
};

#endif
