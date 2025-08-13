#ifndef RUNNABLE_JOIN_GAME_H_
#define RUNNABLE_JOIN_GAME_H_

#include "../../common/commands/join_game.h"
#include "command_runnable_lobby.h"

class RunnableJoinGame : public RunnableCommandLobby {

public:
  RunnableJoinGame(int clt_id, Socket &skt, bool *was_closed)
      : RunnableCommandLobby(std::make_shared<JoinGame>(clt_id, skt, was_closed)) {}

  void
  run(std::shared_ptr<Player> player) override {
    player->manage_join_game(command->get_game_id());
  }

  // uint8_t get_game_id() {
  // 	return ((JoinGame*)command.get())->get_game_id();
  // }
};

#endif
