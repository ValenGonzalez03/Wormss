#ifndef STOP_AIMING_H_
#define STOP_AIMING_H_

#include "command.h"
//#include "protocol.h"
//#include "../server/server_games_handler.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t STOP_AIMING;
}

class StopAiming : public Command {
 private:
 public:
  // Constructor from client side with direction passed by parameter
  explicit StopAiming(uint8_t client_id) : Command(CODE_PLAYER_COMM::STOP_AIMING, client_id) {}

  // Constructor from server side with direction received by socket
  explicit StopAiming(uint8_t clt_id, Socket &skt, bool *was_closed) : Command(CODE_PLAYER_COMM::STOP_AIMING, clt_id) {
    // skt.recvall(&angle, sizeof(angle), was_closed);
  }

  void send(Socket &skt, bool *was_closed) override {
    skt.sendall(&client_id, sizeof(client_id), was_closed);
    skt.sendall(&code, sizeof(code), was_closed);
    // skt.sendall(&angle, sizeof(angle), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override {
    // skt.recvall(&angle, sizeof(angle), was_closed);
  }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }

  // uint8_t get_angle() { return angle; }
};

#endif
