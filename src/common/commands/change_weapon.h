#ifndef CHANGE_WEAPON_H_
#define CHANGE_WEAPON_H_

#include "command.h"

// Forward declaration de CODE_PLAYER_COMM
namespace CODE_PLAYER_COMM {
extern uint8_t CHANGE_WEAPON;
}

class ChangeWeapon : public Command {
 private:
  uint8_t weapon_type;

 public:
  // Constructor from client side with direction passed by parameter
  explicit ChangeWeapon(uint8_t client_id, uint8_t weapon_type) :
      Command(CODE_PLAYER_COMM::CHANGE_WEAPON, client_id), weapon_type(weapon_type) {}

  // Constructor from server side with direction received by socket
  explicit ChangeWeapon(uint8_t clt_id, Socket &skt, bool *was_closed) :  // NOLINT(runtime/references)
      Command(CODE_PLAYER_COMM::CHANGE_WEAPON, clt_id) {
    skt.recvall(&weapon_type, sizeof(weapon_type), was_closed);
  }

  void send(Socket &skt, bool *was_closed) const override {
    skt.sendall(&client_id, sizeof(client_id), was_closed);
    skt.sendall(&code, sizeof(code), was_closed);
    skt.sendall(&weapon_type, sizeof(weapon_type), was_closed);
  }

  void receive(Socket &skt, bool *was_closed) override { skt.recvall(&weapon_type, sizeof(weapon_type), was_closed); }

  // PROVISORIAS
  uint8_t get_client_id() override { return client_id; }

  uint8_t get_game_id() override { return -1; }

  uint8_t get_weapon_type() { return weapon_type; }
};

#endif
