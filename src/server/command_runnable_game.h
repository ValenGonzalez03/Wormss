#ifndef RUNNABLE_COMMAND_GAME_H_
#define RUNNABLE_COMMAND_GAME_H_

#include "command_runnable.h"
#include "game_manager.h"

class RunnableCommandGame : public RunnableCommand {

public:

    RunnableCommandGame(std::shared_ptr<Command> command) : RunnableCommand(command) {}

    virtual void run(GameManager &game_manager) = 0;

};


#endif
