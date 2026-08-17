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
  float charge_power;

 public:
  // Constructor from client side with direction passed by parameter
  explicit Attacking(uint8_t client_id, float charge_power) :
      Command(CODE_PLAYER_COMM::ATTACKING, client_id), charge_power(charge_power) {}

  // Constructor from server side with direction received by socket
  explicit Attacking(uint8_t clt_id, Socket &skt,  // NOLINT(runtime/references)
                     bool *was_closed) : Command(CODE_PLAYER_COMM::ATTACKING, clt_id) {
    int charge_power_int_net;
    skt.recvall(&charge_power_int_net, sizeof(charge_power_int_net), was_closed);
    int charge_power_int = ntohl(charge_power_int_net);
    charge_power = static_cast<float>(charge_power_int) / static_cast<float>(100.0);
  }

  void send(Socket &skt, bool *was_closed) const override {
    skt.sendall(&client_id, sizeof(client_id), was_closed);
    skt.sendall(&code, sizeof(code), was_closed);
    int charge_power_int = static_cast<int>(charge_power * 100);
    int charge_power_int_net = htonl(charge_power_int);
    skt.sendall(&charge_power_int_net, sizeof(charge_power_int_net), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override {
    int charge_power_int_net;
    skt.recvall(&charge_power_int_net, sizeof(charge_power_int_net), was_closed);
    int charge_power_int = ntohl(charge_power_int_net);
    charge_power = static_cast<float>(charge_power_int) / static_cast<float>(100.0);
  }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }

  float get_charge_power() { return charge_power; }
};

#endif
