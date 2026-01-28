#ifndef RUNNABLE_START_AIMING_H_
#define RUNNABLE_START_AIMING_H_

#include "../../common/commands/start_aiming.h"
#include "command_runnable_game.h"
#include <memory>

class RunnableStartAiming : public RunnableCommandGame {
 public:
  explicit RunnableStartAiming(uint8_t clt_id, Socket &skt, bool *was_closed) :  // NOLINT(runtime/references)
      RunnableCommandGame(std::make_shared<StartAiming>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override {
    game_manager.aim(command->get_client_id(), (reinterpret_cast<StartAiming *>(command.get()))->get_direction());
  }
};

#endif
