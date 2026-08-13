#ifndef RUNNABLE_STOP_AIMING_H_
#define RUNNABLE_STOP_AIMING_H_

#include "../../common/commands/stop_aiming.h"
#include "command_runnable_game.h"
#include <memory>

class RunnableStopAiming : public RunnableCommandGame {
 public:
  explicit RunnableStopAiming(uint8_t clt_id, Socket &skt,  // NOLINT(runtime/references)
                              bool *was_closed) :
      RunnableCommandGame(std::make_shared<StopAiming>(clt_id, skt, was_closed)) {}

  void run(Game &game) override { game.stop_aiming(command->get_client_id()); }
};

#endif
