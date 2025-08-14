#ifndef RUNNABLE_COMMAND_LOBBY_H_
#define RUNNABLE_COMMAND_LOBBY_H_

#include "command_runnable.h"
#include "../server_player.h"
#include "../client_manager_thread.h"

class ClientHandler;
class GamesHandler;

class RunnableCommandLobby : public RunnableCommand {
public:
  RunnableCommandLobby(std::shared_ptr<Command> command)
      : RunnableCommand(command) {}

  virtual void
  run(std::shared_ptr<Player> player) = 0;
};

#endif
