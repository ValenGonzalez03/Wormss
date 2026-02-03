#ifndef RUNNABLE_COMMAND_GAME_H_
#define RUNNABLE_COMMAND_GAME_H_

#include "command_runnable.h"
#include <memory>

class GameManager;

class RunnableCommandGame : public RunnableCommand {
 public:
  explicit RunnableCommandGame(std::shared_ptr<Command> command) : RunnableCommand(command) {}

  virtual void run(GameManager &game_manager) = 0;  // NOLINT(runtime/references)
};

typedef std::shared_ptr<RunnableCommandGame> game_command_ptr;

#endif
