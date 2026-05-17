#ifndef STOP_GAME_H_
#define STOP_GAME_H_

#include "command.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t STOP_GAME;
}

class StopGame : public Command {
 public:
  // Constructor from client side
  explicit StopGame(uint8_t client_id) : Command(CODE_PLAYER_COMM::STOP_GAME, client_id) {}

  // Constructor from server side for code consistency but doesn't do anything
  // different from the other
  explicit StopGame(uint8_t clt_id, Socket &skt, bool *was_closed) :
      Command(CODE_PLAYER_COMM::STOP_GAME, clt_id) {
    // receive(skt, was_closed);
  }

  void send(Socket &skt, bool *was_closed) const override {
    skt.sendall(&client_id, sizeof(client_id), was_closed);
    skt.sendall(&code, sizeof(code), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override {}

  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }
};

#endif
