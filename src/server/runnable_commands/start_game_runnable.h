#ifndef RUNNABLE_START_GAME_H_
#define RUNNABLE_START_GAME_H_

#include "../../common/commands/start_game.h"
#include "command_runnable_lobby.h"
#include "../comm/client_manager_thread.h"
#include <memory>

class RunnableStartGame : public RunnableCommandLobby {
 public:
  RunnableStartGame(int clt_id, Socket &skt, bool *was_closed) :  // NOLINT(runtime/references)
      RunnableCommandLobby(std::make_shared<StartGame>(clt_id, skt, was_closed)) {}

  void run(ClientManager &client_manager) override {
    client_manager.manage_start_game(command->get_game_id());
  }
};

#endif
