#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include <vector>
#include <cstdint>

class TurnManager {
 private:
  std::vector<uint8_t> *player_ids;
  int current_index = 0;
  float time_remaining;

 public:
  explicit TurnManager(std::vector<uint8_t> *ids);

  void update(float dt);
  void advance_turn();

  uint8_t get_current_turn_id() const;
  float get_time_remaining() const;
  bool is_turn_of(uint8_t player_id) const;
};

#endif
