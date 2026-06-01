#ifndef RUNNABLE_GAME_STARTED_H_
#define RUNNABLE_GAME_STARTED_H_

#include "../../common/commands/game_started.h"
#include "command_runnable_lobby.h"
#include "../comm/client_manager_thread.h"
#include <memory>

class RunnableGameStarted : public RunnableCommandLobby {
 public:
  RunnableGameStarted(int clt_id, Socket &skt,  // NOLINT(runtime/references)
                      bool *was_closed) :
      RunnableCommandLobby(std::make_shared<GameStarted>(clt_id, skt, was_closed)) {}

  // Comando creado con el fin de desbloquear el Receiver del Server.
  void run(ClientManager &client_manager) override {
    client_manager.manage_game_started();
  }
};

#endif
