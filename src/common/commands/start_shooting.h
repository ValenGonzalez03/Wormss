#ifndef START_SHOOTING_H_
#define START_SHOOTING_H_

#include "command.h"
#include <arpa/inet.h>
//#include "protocol.h"
//#include "../server/server_games_handler.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t START_SHOOTING;
}

class StartShooting : public Command {
private:
  float initial_force;

public:
  // Constructor from client side with direction passed by parameter
  explicit StartShooting(float initial_force)
      : Command(CODE_PLAYER_COMM::START_SHOOTING, 0), initial_force(initial_force) {}

  // Constructor from server side with direction received by socket
  explicit StartShooting(uint8_t clt_id, Socket &skt, bool *was_closed)
      : Command(CODE_PLAYER_COMM::START_SHOOTING, clt_id) {
      int initial_force_int_net;
      skt.recvall(&initial_force_int_net, sizeof(initial_force_int_net), was_closed);
      int initial_force_int = ntohl(initial_force_int_net);
      initial_force = float(initial_force_int) / float(100.0);
  }

  void send(Socket &skt, bool *was_closed) override {
    skt.sendall(&code, sizeof(code), was_closed);
    int initial_force_int = int(initial_force * 100);
    int initial_force_int_net = htonl(initial_force_int);
    skt.sendall(&initial_force_int_net, sizeof(initial_force_int_net), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override {
    int initial_force_int_net;
    skt.recvall(&initial_force_int_net, sizeof(initial_force_int_net), was_closed);
    int initial_force_int = ntohl(initial_force_int_net);
    initial_force = float(initial_force_int) / float(100.0);
  }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }

  uint8_t get_initial_force() { return initial_force; }
};

#endif
