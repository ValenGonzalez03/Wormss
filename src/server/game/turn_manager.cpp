#include "turn_manager.h"
#include <iostream>

#define TURN_DURATION 20.0f

TurnManager::TurnManager(std::vector<uint8_t> *ids) :
    player_ids(ids), current_index(0), time_remaining(TURN_DURATION) {}

void TurnManager::update(float dt) {
  time_remaining -= dt;
  if (time_remaining <= 0.0f) {
    advance_turn();
  }
}

void TurnManager::advance_turn() {
  current_index = (current_index + 1) % player_ids->size();
  time_remaining = TURN_DURATION;
}

uint8_t TurnManager::get_current_turn_id() const { return player_ids->at(current_index); }

float TurnManager::get_time_remaining() const { return time_remaining; }

bool TurnManager::is_turn_of(uint8_t player_id) const {
  return player_ids->at(current_index) == player_id;
}
