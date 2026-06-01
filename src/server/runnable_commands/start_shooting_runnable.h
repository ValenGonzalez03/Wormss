#ifndef RUNNABLE_START_SHOOTING_H_
#define RUNNABLE_START_SHOOTING_H_

#include "../../common/commands/start_shooting.h"
#include "command_runnable_game.h"
#include <memory>

class RunnableStartShooting : public RunnableCommandGame {
 public:
  explicit RunnableStartShooting(uint8_t clt_id,
                                 Socket &skt,  // NOLINT(runtime/references)
                                 bool *was_closed) :
      RunnableCommandGame(std::make_shared<StartShooting>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override {
    game_manager.attack(
        command->get_client_id(),
        (reinterpret_cast<StartShooting *>(command.get()))->get_initial_force());
  }
};

#endif
