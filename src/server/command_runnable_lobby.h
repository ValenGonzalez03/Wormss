#ifndef RUNNABLE_COMMAND_LOBBY_H_
#define RUNNABLE_COMMAND_LOBBY_H_

#include "command_runnable.h"
//#include "../server/client_handler_thread.h"

class RunnableCommandLobby : RunnableCommand {

public:

    RunnableCommandLobby(std::shared_ptr<Command> command) : RunnableCommand(command) {}

    virtual void run(/* Alguna clase de server para comandos de lobby*/) = 0;

};

#endif
