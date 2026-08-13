#ifndef RUNNABLE_JUMP_H_
#define RUNNABLE_JUMP_H_

#include "../../common/commands/jump.h"
#include "command_runnable_game.h"
#include <memory>

class RunnableJump : public RunnableCommandGame {
 public:
  explicit RunnableJump(int clt_id, Socket &skt,  // NOLINT(runtime/references)
                        bool *was_closed) :
      RunnableCommandGame(std::make_shared<Jump>(clt_id, skt, was_closed)) {}

  void run(Game &game) override {
    game.jump(command->get_client_id(),
              (reinterpret_cast<Jump *>(command.get()))->get_direction(),
              (reinterpret_cast<Jump *>(command.get()))->get_jump_type());
  }
};

#endif
