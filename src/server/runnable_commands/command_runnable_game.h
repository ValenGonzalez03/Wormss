#ifndef RUNNABLE_COMMAND_GAME_H_
#define RUNNABLE_COMMAND_GAME_H_

#include "command_runnable.h"

class GameManager;

class RunnableCommandGame : public RunnableCommand {

public:
  RunnableCommandGame(std::shared_ptr<Command> command)
      : RunnableCommand(command) {}

  virtual void run(GameManager &game_manager) = 0;
};

#endif
