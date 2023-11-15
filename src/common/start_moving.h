#ifndef START_MOVING_H_
#define START_MOVING_H_

#include "command.h"
#include "protocol.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t START_MOVING;
}

class StartMoving : public Command {
private:
  uint8_t direction;

public:
  // Constructor from client side with direction passed by parameter
  explicit StartMoving(int dir)
      : Command(CODE_PLAYER_COMM::START_MOVING, 0), direction(dir) {}

  // Constructor from server side with direction received by socket
  StartMoving(int clt_id, Socket &skt, bool *was_closed)
      : Command(CODE_PLAYER_COMM::START_MOVING, clt_id) {
    skt.recvall(&direction, sizeof(direction), was_closed);
  }

  void send(Socket &skt, bool *was_closed) override {
    skt.sendall(&code, sizeof(code), was_closed);
    skt.sendall(&direction, sizeof(direction), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override {
    skt.recvall(&direction, sizeof(direction), was_closed);
  }

  void run() override { std::cout << "Start moving" << std::endl; }
};

#endif