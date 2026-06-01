#ifndef JUMP_H_
#define JUMP_H_

#include "command.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t JUMP;
}

class Jump : public Command {
 private:
  uint8_t direction;
  uint8_t jump_type;

 public:
  // Constructor from client side with direction passed by parameter
  explicit Jump(uint8_t client_id, uint8_t dir, uint8_t jump_type) :
      Command(CODE_PLAYER_COMM::JUMP, client_id), direction(dir), jump_type(jump_type) {}

  // Constructor from server side with direction received by socket
  explicit Jump(uint8_t clt_id, Socket &skt,  // NOLINT(runtime/references)
                bool *was_closed) : Command(CODE_PLAYER_COMM::JUMP, clt_id) {
    skt.recvall(&direction, sizeof(direction), was_closed);
    skt.recvall(&jump_type, sizeof(jump_type), was_closed);
  }

  void send(Socket &skt, bool *was_closed) const override {
    skt.sendall(&client_id, sizeof(client_id), was_closed);
    skt.sendall(&code, sizeof(code), was_closed);
    skt.sendall(&direction, sizeof(direction), was_closed);
    skt.sendall(&jump_type, sizeof(jump_type), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override {
    skt.recvall(&direction, sizeof(direction), was_closed);
    skt.recvall(&jump_type, sizeof(jump_type), was_closed);
  }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }

  uint8_t get_direction() { return direction; }

  uint8_t get_jump_type() { return jump_type; }
};

#endif
