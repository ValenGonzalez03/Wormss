#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include <map>
#include <cstdint>
#include "server_player.h"

class TurnManager {
 private:
  std::map<uint8_t, Player> *players;
  std::map<uint8_t, Player>::iterator current_player_it;
  // int current_turn_id;
  float time_remaining;

 public:
  explicit TurnManager(std::map<uint8_t, Player> *players);

  void update(float dt);
  void advance_turn();
  void assign_first_player();
  bool is_turn_of(uint8_t player_id) const;

  Player &get_current_player() const;
  float get_time_remaining() const;
};

#endif
