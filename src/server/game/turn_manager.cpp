#include "turn_manager.h"
#include <iostream>

#define TURN_DURATION 20.0f

TurnManager::TurnManager(std::map<uint8_t, Player>* players) :
    players(players), time_remaining(TURN_DURATION) {}

void TurnManager::update(float dt) {
  time_remaining -= dt;
  if (time_remaining <= 0.0f) {
    advance_turn();
  }
}

void TurnManager::advance_turn() {
  if (players->empty()) {
    return;
  }

  current_player_it->second.advance_worm();
  ++current_player_it;
  if (current_player_it == players->end()) {
    current_player_it = players->begin();
  }

  time_remaining = TURN_DURATION;
}

bool TurnManager::is_turn_of(uint8_t player_id) const {
  return current_player_it->first == player_id;
}

void TurnManager::assign_first_player() {
  if (players->empty()) {
    return;
  }
  current_player_it = players->begin();
}

Player& TurnManager::get_current_player() const { return current_player_it->second; }

float TurnManager::get_time_remaining() const { return time_remaining; }
