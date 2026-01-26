#ifndef RUNNABLE_START_GAME_H_
#define RUNNABLE_START_GAME_H_

#include "../../common/commands/start_game.h"
#include "command_runnable_game.h"
#include "command_runnable_lobby.h"

class RunnableStartGame : public RunnableCommandLobby {

 public:
  RunnableStartGame(int clt_id, Socket &skt, bool *was_closed) :
      RunnableCommandLobby(std::make_shared<StartGame>(clt_id, skt, was_closed)) {}

  void run(std::shared_ptr<Player> player) override { player->manage_start_game(); }
};

#endif
