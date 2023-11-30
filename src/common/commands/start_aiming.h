#ifndef START_AIMING_H_
#define START_AIMING_H_

#include "command.h"
//#include "protocol.h"
//#include "../server/server_games_handler.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t START_AIMING;
}

class StartAiming : public Command {
private:
  uint8_t direction;

public:
  // Constructor from client side with direction passed by parameter
  explicit StartAiming(uint8_t dir)
      : Command(CODE_PLAYER_COMM::START_AIMING, 0), direction(dir) {}

  // Constructor from server side with direction received by socket
  explicit StartAiming(uint8_t clt_id, Socket &skt, bool *was_closed)
      : Command(CODE_PLAYER_COMM::START_AIMING, clt_id) {
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
