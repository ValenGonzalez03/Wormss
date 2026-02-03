#ifndef RUNNABLE_JOIN_GAME_H_
#define RUNNABLE_JOIN_GAME_H_

#include "../../common/commands/join_game.h"
#include "command_runnable_lobby.h"
#include "../comm/client_manager_thread.h"
#include <memory>

class RunnableJoinGame : public RunnableCommandLobby {
 public:
  RunnableJoinGame(int clt_id, Socket &skt, bool *was_closed) :  // NOLINT(runtime/references)
      RunnableCommandLobby(std::make_shared<JoinGame>(clt_id, skt, was_closed)) {}

  void run(ClientManager &client_manager) override { client_manager.manage_join_game(command->get_game_id()); }
};

#endif
