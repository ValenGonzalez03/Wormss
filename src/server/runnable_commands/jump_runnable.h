#ifndef RUNNABLE_JUMP_H_
#define RUNNABLE_JUMP_H_

#include "../../common/commands/jump.h"
#include "command_runnable_game.h"
#include <memory>

class RunnableJump : public RunnableCommandGame {
 public:
  explicit RunnableJump(int clt_id, Socket &skt, bool *was_closed) :  // NOLINT(runtime/references)
      RunnableCommandGame(std::make_shared<Jump>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override {
    game_manager.jump(command->get_client_id(),
                      (reinterpret_cast<Jump *>(command.get()))->get_direction(),
                      (reinterpret_cast<Jump *>(command.get()))->get_jump_type());
  }
};

#endif
