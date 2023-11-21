#ifndef RUNNABLE_COMMAND_H_
#define RUNNABLE_COMMAND_H_

#include "../common/command.h"
#include "../common/protocol.h"

class RunnableCommand {

protected:

std::shared_ptr<Command> command;

public:

    RunnableCommand(std::shared_ptr<Command> command) : command(command) {}

    void send(Socket &skt, bool* was_closed) {
        command->send(skt, was_closed);
    }
    
    void receive(Socket &skt, bool* was_closed) {
        command->receive(skt, was_closed);
    }

};


#endif
