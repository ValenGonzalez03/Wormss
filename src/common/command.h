#ifndef COMMAND_H_
#define COMMAND_H_

#include <cstdint>
#include "socket.h"

class Command {
protected:
    uint8_t code;
    uint8_t client_id;

public:
    Command(int cod, int clt_id) : code(cod), client_id(clt_id) {}

    // Debe enviar el codigo y los datos adicionales
    // de ser necesario
    virtual void send(Socket &skt, bool* was_closed) = 0;
    
    // El código lo recibe el protocolo para decidir
    // el comando a crear. Este receive solo recibe los datos
    // adicionales del comando en caso de ser necesario.
    virtual void receive(Socket &skt, bool* was_closed) = 0;

    virtual void run() = 0;
};

#endif