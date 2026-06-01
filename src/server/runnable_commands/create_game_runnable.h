#ifndef RUNNABLE_CREATE_GAME_H_
#define RUNNABLE_CREATE_GAME_H_

#include "../../common/commands/create_game.h"
#include "command_runnable_lobby.h"
#include "../comm/client_manager_thread.h"
#include <memory>


class RunnableCreateGame : public RunnableCommandLobby {
 public:
  RunnableCreateGame(int clt_id, Socket &skt,  // NOLINT(runtime/references)
                     bool *was_closed) :
      RunnableCommandLobby(std::make_shared<CreateGame>(clt_id, skt, was_closed)) {}

  void run(ClientManager &client_manager) override {
    client_manager.manage_create_game();
  }
};

#endif
