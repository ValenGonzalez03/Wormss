#ifndef RUNNABLE_CREATE_GAME_H_
#define RUNNABLE_CREATE_GAME_H_

#include "../../common/commands/create_game.h"
#include "command_runnable_lobby.h"


class RunnableCreateGame : public RunnableCommandLobby {

 public:
  RunnableCreateGame(int clt_id, Socket &skt, bool *was_closed) :
      RunnableCommandLobby(std::make_shared<CreateGame>(clt_id, skt, was_closed)) {}

  void run(std::shared_ptr<Player> player) override { player->manage_create_game(); }
};

#endif
