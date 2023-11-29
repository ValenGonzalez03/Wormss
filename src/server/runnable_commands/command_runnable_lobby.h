#ifndef RUNNABLE_COMMAND_LOBBY_H_
#define RUNNABLE_COMMAND_LOBBY_H_

#include "../lobby_result.h"
#include "../server_games_handler.h"
#include "command_runnable.h"

class RunnableCommandLobby : public RunnableCommand {
public:
  RunnableCommandLobby(std::shared_ptr<Command> command)
      : RunnableCommand(command) {}

  virtual std::unique_ptr<LobbyResult>
  run(GamesHandler &games_handler, 
      std::shared_ptr<Queue<GameState>> sender_queue, uint8_t &player_id) = 0;
};

#endif
