#ifndef RUNNABLE_ATTACKING_H_
#define RUNNABLE_ATTACKING_H_

#include "../../common/commands/attacking.h"
#include "command_runnable_game.h"
#include <memory>

class RunnableAttacking : public RunnableCommandGame {
 public:
  explicit RunnableAttacking(uint8_t clt_id, Socket &skt,  // NOLINT(runtime/references)
                             bool *was_closed) :
      RunnableCommandGame(std::make_shared<Attacking>(clt_id, skt, was_closed)) {}

  void run(Game &game) override {
    game.attack(command->get_client_id(),
                (reinterpret_cast<Attacking *>(command.get()))->get_charge_intensity());
  }
};

#endif
