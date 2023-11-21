#ifndef COMMAND_H_
#define COMMAND_H_

#include <cstdint>
#include "socket.h"
#include "queue.h"

class Command {
protected:
    uint8_t code ;
    uint8_t client_id;

public:
    Command(int cod, int clt_id) : code(cod), client_id(clt_id) {}

    // Debe enviar el codigo y los datos adicionales
    // de ser necesario
    virtual void send(Socket &skt, bool* was_closed) = 0;
    
    #ifdef SERVER_BUILD
    // El código lo recibe el protocolo para decidir
    // el comando a crear. Este receive solo recibe los datos
    // adicionales del comando en caso de ser necesario.
    virtual void receive(Socket &skt, bool* was_closed) = 0;
    
    // PROVISORIAS
    virtual uint8_t get_client_id() = 0;
    virtual uint8_t get_game_id() = 0;
    #endif
};

#endif
