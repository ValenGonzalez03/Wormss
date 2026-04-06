#ifndef GAME_STARTED_H_
#define GAME_STARTED_H_

#include "command.h"

namespace CODE_PLAYER_COMM {
extern uint8_t GAME_STARTED;
}

class GameStarted : public Command {
 private:
  uint8_t game_id;  // uint8_t game_id  CAMBIAR !?

 public:
  // Constructor from client side
  explicit GameStarted(int game_id) : game_id(game_id), Command(CODE_PLAYER_COMM::GAME_STARTED, 0) {}

  // Constructor from server side
  explicit GameStarted(uint8_t clt_id, Socket &skt, bool *was_closed) : Command(CODE_PLAYER_COMM::GAME_STARTED, clt_id) {
    skt.recvall(&game_id, sizeof(game_id), was_closed);
  }

  void send(Socket &skt, bool *was_closed) const override {
    skt.sendall(&code, sizeof(code), was_closed);
    skt.sendall(&game_id, sizeof(game_id), was_closed);
  }
  void receive(Socket &skt, bool *was_closed) override { skt.recvall(&game_id, sizeof(game_id), was_closed); }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return game_id; }
};

#endif
