#ifndef RUNNABLE_CHANGE_WEAPON_H_
#define RUNNABLE_CHANGE_WEAPON_H_

#include "../../common/commands/change_weapon.h"
#include "command_runnable_game.h"
#include <memory>

class RunnableChangeWeapon : public RunnableCommandGame {
 public:
  explicit RunnableChangeWeapon(uint8_t clt_id,
                                Socket &skt,  // NOLINT(runtime/references)
                                bool *was_closed) :
      RunnableCommandGame(std::make_shared<ChangeWeapon>(clt_id, skt, was_closed)) {}

  void run(Game &game) override {
    game.change_weapon(
        command->get_client_id(),
        (reinterpret_cast<ChangeWeapon *>(command.get()))->get_weapon_type());
  }
};

#endif
