#include "server_player.h"
#include <algorithm>

Player::Player(uint8_t player_id) : player_id(player_id) {}

void Player::add_worm(WormBody *worm) {
  worms[worm->get_id()] = worm;
  if (worms.size() == 1)
    current_worm_it = worms.begin();
}

void Player::remove_worm(WormBody *worm) {
  worms.erase(worm->get_id());
  if (!worms.empty())
    current_worm_it = worms.begin();
}

void Player::advance_worm() {
  if (worms.empty()) {
    return;
  }

  ++current_worm_it;
  if (current_worm_it == worms.end()) {
    current_worm_it = worms.begin();
  }
}

bool Player::has_lost() const { return worms.empty(); }

uint8_t Player::get_player_id() const { return player_id; }
WormBody *Player::get_current_worm() const { return current_worm_it->second; }
