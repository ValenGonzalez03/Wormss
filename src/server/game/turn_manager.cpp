#include "turn_manager.h"
#include <iostream>

#define TURN_DURATION 10.0f

TurnManager::TurnManager(std::map<uint8_t, Player>* players) :
    players(players), time_remaining(TURN_DURATION) {}

void TurnManager::update(const float dt) {
  if (players->empty()) {
    return;
  }
  Player& current_player = current_player_it->second;
  WormBody* current_worm = current_player.get_current_worm();
  time_remaining -= dt;
  if (time_remaining <= 0.0f) {  // Termino el tiempo del turno
    current_player_it->second.advance_worm();
    next_turn();
  } else if (current_worm->is_dead()) {  // Un gusano ha muerto durante el turno
    current_player.remove_worm(current_worm->get_id());
    std::cout << "[GAME-THREAD]: Gusano " << static_cast<int>(current_worm->get_id())
              << " del jugador " << static_cast<int>(current_player.get_player_id())
              << " eliminado del juego." << std::endl;
    if (current_player
            .has_lost()) {  // Si ademas el gusano que murió era el ultimo del jugador, se elimina al jugador
      remove_player(current_player_it->first);
      std::cout << "[GAME-THREAD]: Jugador " << static_cast<int>(current_player_it->first)
                << " eliminado del juego." << std::endl;
    }
    next_turn();
  }
}

void TurnManager::next_turn() {
  ++current_player_it;
  if (current_player_it == players->end()) {
    current_player_it = players->begin();
  }
  time_remaining = TURN_DURATION;
}

void TurnManager::remove_player(const uint8_t player_id) {
  auto it = players->find(player_id);
  if (it != players->end()) {
    current_player_it = players->erase(it);
  }
}

bool TurnManager::is_turn_of(uint8_t player_id) const {
  return current_player_it->first == player_id;
}

bool TurnManager::are_players_remaining() const { return !(players->empty()); }

void TurnManager::assign_first_player() {
  if (players->empty()) {
    return;
  }
  current_player_it = players->begin();
}

Player& TurnManager::get_current_player() const { return current_player_it->second; }

float TurnManager::get_time_remaining() const { return time_remaining; }
