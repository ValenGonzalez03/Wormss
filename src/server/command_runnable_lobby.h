#ifndef RUNNABLE_COMMAND_LOBBY_H_
#define RUNNABLE_COMMAND_LOBBY_H_

#include "command_runnable.h"
#include "client_handler_thread.h"

//class ClientHandler;

class RunnableCommandLobby : public RunnableCommand {
public:

    RunnableCommandLobby(std::shared_ptr<Command> command) : RunnableCommand(command) {}

    virtual void run(ClientHandler& client_handler) = 0;
};

#endif
