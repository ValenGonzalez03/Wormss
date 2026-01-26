#ifndef RUNNABLE_START_SHOOTING_H_
#define RUNNABLE_START_SHOOTING_H_

#include "../../common/commands/start_shooting.h"
#include "command_runnable_game.h"

class RunnableStartShooting : public RunnableCommandGame {

 public:
  explicit RunnableStartShooting(uint8_t clt_id, Socket &skt, bool *was_closed) :
      RunnableCommandGame(std::make_shared<StartShooting>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override {
    game_manager.attack(command->get_client_id(), ((StartShooting *)command.get())->get_initial_force());
  }
};

#endif
