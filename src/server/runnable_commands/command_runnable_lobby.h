#ifndef RUNNABLE_COMMAND_LOBBY_H_
#define RUNNABLE_COMMAND_LOBBY_H_

#include "command_runnable.h"
// #include "../comm/client_manager_thread.h"
#include <memory>


class ClientManager;

class RunnableCommandLobby : public RunnableCommand {
 public:
  explicit RunnableCommandLobby(std::shared_ptr<Command> command) :
      RunnableCommand(command) {}

  virtual void run(ClientManager& client_manager) = 0;  // NOLINT(runtime/references)
};

typedef std::shared_ptr<RunnableCommandLobby> lobby_command_ptr;

#endif
