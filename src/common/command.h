#ifndef COMMAND_H_
#define COMMAND_H_

#include <cstdint>
#include "socket.h"
#include "queue.h"
//#include "../server/server_games_handler.h"

class Command {
protected:
    uint8_t code ;
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

    //virtual Queue<std::shared_ptr<Command>>* run(GamesHandler& games_handler, std::shared_ptr<Queue<GameState>> sender_queue) = 0;
    
    virtual bool is_connect_type() = 0;
    
    virtual bool is_create_command() = 0;
    
    virtual bool is_join_command() = 0;
    
    // PROVISORIAS
    virtual uint8_t get_client_id() = 0;
    virtual uint8_t get_game_id() = 0;
};

#endif
