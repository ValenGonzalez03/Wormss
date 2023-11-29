#ifndef START_GAME_H_
#define START_GAME_H_

#include "command.h"
//#include "protocol.h"

namespace CODE_PLAYER_COMM {
extern uint8_t START_GAME;
}

class StartGame : public Command {
private:
  uint8_t game_id; // uint8_t game_id  CAMBIAR !?

public:
  // Constructor from client side
  explicit StartGame(int game_id)
      : game_id(game_id), Command(CODE_PLAYER_COMM::START_GAME, 0) {}

  // Constructor from server side
  explicit StartGame(uint8_t clt_id, Socket &skt, bool *was_closed)
      : Command(CODE_PLAYER_COMM::START_GAME, clt_id) {
    receive(skt, was_closed);
  }

  void send(Socket &skt, bool *was_closed) override {
    skt.sendall(&code, sizeof(code), was_closed);
    skt.sendall(&game_id, sizeof(game_id), was_closed);
  }
  void receive(Socket &skt, bool *was_closed) override {
    skt.recvall(&game_id, sizeof(game_id), was_closed);
  }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return game_id; }
};

#endif
