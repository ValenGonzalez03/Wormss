#ifndef CREATE_GAME_H_
#define CREATE_GAME_H_

#include "command.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t CREATE_GAME;
}

class CreateGame : public Command {
 public:
  // Constructor from client side
  CreateGame() : Command(CODE_PLAYER_COMM::CREATE_GAME, 0) {}

  // Constructor from server side
  explicit CreateGame(uint8_t clt_id, Socket &skt,  // NOLINT(runtime/references)
                      bool *was_closed) : Command(CODE_PLAYER_COMM::CREATE_GAME, clt_id) {
    // receive(skt, was_closed);
  }

  void send(Socket &skt, bool *was_closed) const override {  // NOLINT(runtime/references)
    skt.sendall(&code, sizeof(code), was_closed);
  }  // NOLINT(runtime/references)

  void receive(Socket &skt, bool *was_closed) override {}

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }
};

#endif
