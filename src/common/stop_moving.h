#ifndef STOP_MOVING_H_
#define STOP_MOVING_H_

#include "command.h"
#include "protocol.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
    extern uint8_t STOP_MOVING;
}

class StopMoving : public Command {
public:
    // Constructor from client side
    StopMoving(int clt_id) : Command(CODE_PLAYER_COMM::STOP_MOVING, clt_id) {}

    // Constructor from server side for code consistency but doesn't do anything
    // different from the other
    StopMoving(int clt_id, Socket &skt, bool* was_closed) : Command(CODE_PLAYER_COMM::STOP_MOVING, clt_id) {
        receive(skt, was_closed);
    }

    void send(Socket &skt, bool* was_closed) override {
        skt.sendall(&code, sizeof(code), was_closed);
        skt.sendall(&client_id, sizeof(client_id), was_closed);
    }

    void receive(Socket &skt, bool* was_closed) override {}

    void run() {
        std::cout << "Stop Moving" << std::endl;
    }
};

#endif