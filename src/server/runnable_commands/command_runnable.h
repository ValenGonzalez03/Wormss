#ifndef RUNNABLE_COMMAND_H_
#define RUNNABLE_COMMAND_H_

#include "../../common/commands/command.h"
#include <memory>

class RunnableCommand {
 protected:
  std::shared_ptr<Command> command;

 public:
  explicit RunnableCommand(std::shared_ptr<Command> command) : command(command) {}

  void send(Socket &skt, bool *was_closed) { command->send(skt, was_closed); }  // NOLINT(runtime/references)

  void receive(Socket &skt, bool *was_closed) { command->receive(skt, was_closed); }  // NOLINT(runtime/references)
};

#endif
