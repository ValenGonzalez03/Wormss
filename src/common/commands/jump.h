#ifndef JUMP_H_
#define JUMP_H_

#include "command.h"
//#include "protocol.h"
//#include "../server/server_games_handler.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t JUMP;
}

class Jump : public Command {
private:
  uint8_t direction;

public:
  // Constructor from client side with direction passed by parameter
  explicit Jump(uint8_t dir)
      : Command(CODE_PLAYER_COMM::JUMP, 0), direction(dir) {}

  // Constructor from server side with direction received by socket
  explicit Jump(uint8_t clt_id, Socket &skt, bool *was_closed)
      : Command(CODE_PLAYER_COMM::JUMP, clt_id) {
    skt.recvall(&direction, sizeof(direction), was_closed);
  }

  void send(Socket &skt, bool *was_closed) override {
    skt.sendall(&code, sizeof(code), was_closed);
    skt.sendall(&direction, sizeof(direction), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override {
    skt.recvall(&direction, sizeof(direction), was_closed);
  }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }

  uint8_t get_direction() { return direction; }
};

#endif
