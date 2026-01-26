#ifndef RUNNABLE_STOP_AIMING_H_
#define RUNNABLE_STOP_AIMING_H_

#include "../../common/commands/stop_aiming.h"
#include "command_runnable_game.h"

class RunnableStopAiming : public RunnableCommandGame {

 public:
  explicit RunnableStopAiming(uint8_t clt_id, Socket &skt, bool *was_closed) :
      RunnableCommandGame(std::make_shared<StopAiming>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override { game_manager.stop_aiming(command->get_client_id()); }
};

#endif
