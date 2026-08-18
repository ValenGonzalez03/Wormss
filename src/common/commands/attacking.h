#ifndef ATTACKING_H_
#define ATTACKING_H_

#include "command.h"
#include <arpa/inet.h>

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t ATTACKING;
}

class Attacking : public Command {
 private:
  float charge_intensity;

 public:
  // Constructor from client side with direction passed by parameter
  explicit Attacking(uint8_t client_id, float charge_intensity) :
      Command(CODE_PLAYER_COMM::ATTACKING, client_id),
      charge_intensity(charge_intensity) {}

  // Constructor from server side with direction received by socket
  explicit Attacking(uint8_t clt_id, Socket &skt,  // NOLINT(runtime/references)
                     bool *was_closed) : Command(CODE_PLAYER_COMM::ATTACKING, clt_id) {
    int charge_intensity_int_net;
    skt.recvall(&charge_intensity_int_net, sizeof(charge_intensity_int_net), was_closed);
    int charge_intensity_int = ntohl(charge_intensity_int_net);
    charge_intensity =
        static_cast<float>(charge_intensity_int) / static_cast<float>(100.0);
  }

  void send(Socket &skt, bool *was_closed) const override {
    skt.sendall(&client_id, sizeof(client_id), was_closed);
    skt.sendall(&code, sizeof(code), was_closed);
    int charge_intensity_int = static_cast<int>(charge_intensity * 100);
    int charge_intensity_int_net = htonl(charge_intensity_int);
    skt.sendall(&charge_intensity_int_net, sizeof(charge_intensity_int_net), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override {
    int charge_intensity_int_net;
    skt.recvall(&charge_intensity_int_net, sizeof(charge_intensity_int_net), was_closed);
    int charge_intensity_int = ntohl(charge_intensity_int_net);
    charge_intensity =
        static_cast<float>(charge_intensity_int) / static_cast<float>(100.0);
  }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }

  float get_charge_intensity() { return charge_intensity; }
};

#endif
