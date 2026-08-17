#ifndef RUNNABLE_CHARGING_ATTACK_H_
#define RUNNABLE_CHARGING_ATTACK_H_

#include "../../common/commands/charging_attack.h"
#include "command_runnable_game.h"
#include <memory>

class RunnableChargingAttack : public RunnableCommandGame {
 public:
  RunnableChargingAttack(uint8_t clt_id, Socket &skt,  // NOLINT(runtime/references)
                         bool *was_closed) :
      RunnableCommandGame(std::make_shared<ChargingAttack>(clt_id, skt, was_closed)) {}

  void run(Game &game) override { game.set_worm_to_charge(command->get_client_id()); }
};

#endif
