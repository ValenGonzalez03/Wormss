#ifndef RUNNABLE_COMMAND_LOBBY_H_
#define RUNNABLE_COMMAND_LOBBY_H_

#include "command_runnable.h"
#include "../comm/server_player.h"
#include "../comm/client_manager_thread.h"
#include <memory>

class ClientHandler;
class GamesHandler;

class RunnableCommandLobby : public RunnableCommand {
 public:
  explicit RunnableCommandLobby(std::shared_ptr<Command> command) : RunnableCommand(command) {}

  virtual void run(std::shared_ptr<Player> player) = 0;
};

#endif
