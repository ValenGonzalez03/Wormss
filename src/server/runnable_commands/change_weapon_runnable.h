#ifndef RUNNABLE_CHANGE_WEAPON_H_
#define RUNNABLE_CHANGE_WEAPON_H_

#include "../../common/commands/change_weapon.h"
#include "command_runnable_game.h"
#include "../game_manager.h"

class RunnableChangeWeapon : public RunnableCommandGame {

public:
  explicit RunnableChangeWeapon(uint8_t clt_id, Socket &skt, bool *was_closed)
      : RunnableCommandGame(std::make_shared<ChangeWeapon>(clt_id, skt, was_closed)) {}

  void run(GameManager &game_manager) override {
    game_manager.change_weapon(command->get_client_id(), ((ChangeWeapon *)command.get())->get_weapon_type());
  }
};

#endif
