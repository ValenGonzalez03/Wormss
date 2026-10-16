#include "turn_manager.h"
#include <iostream>

#define TURN_DURATION 10.0f
#define POST_ATTACK_GRACE_DURATION 3.0f

TurnManager::TurnManager(std::map<uint8_t, Player>* players) :
    players(players), time_remaining(TURN_DURATION) {}

void TurnManager::update(const float dt) {
  if (players->empty()) {
    return;
  }

  Player& current_player = current_player_it->second;
  WormBody* current_worm = current_player.get_current_worm();

  if (current_worm->is_dead()) {  // Un gusano ha muerto durante el turno
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

  switch (state) {
    case TurnState::ACTIVE:
      time_remaining -= dt;
      if (current_worm->has_taken_damage()) {
        on_damage();
      } else if (time_remaining <= 0.0f) {
        state = TurnState::FINISHED;
        current_worm->reset_state();
      }
      break;

    case TurnState::POST_ATTACK_GRACE:
      post_attack_grace_remaining -= dt;
      if (post_attack_grace_remaining <= 0.0f) {
        state = TurnState::FINISHED;
        current_worm->reset_state();
      }
      break;

    case TurnState::FINISHED:
      break;
  }
}

void TurnManager::next_turn() {
  ++current_player_it;
  if (current_player_it == players->end()) {
    current_player_it = players->begin();
  }
  current_player_it->second.get_current_worm()->reset_damage_flag();
  time_remaining = TURN_DURATION;
  post_attack_grace_remaining = 0.0f;
  state = TurnState::ACTIVE;
}

void TurnManager::advance_full_turn() {
  current_player_it->second.advance_worm();
  next_turn();
}

void TurnManager::on_attack() {
  state = TurnState::POST_ATTACK_GRACE;
  post_attack_grace_remaining = POST_ATTACK_GRACE_DURATION;
}

void TurnManager::on_damage() {
  state = TurnState::FINISHED;
  current_player_it->second.get_current_worm()->reset_damage_flag();
}

bool TurnManager::ended_his_turn() const { return state == TurnState::FINISHED; }

bool TurnManager::can_play_in_turn(uint8_t player_id) const {
  return current_player_it->first == player_id && (state != TurnState::FINISHED);
}

void TurnManager::assign_first_player() {
  if (players->empty()) {
    return;
  }
  current_player_it = players->begin();
}

void TurnManager::remove_player(const uint8_t player_id) {
  auto it = players->find(player_id);
  if (it != players->end()) {
    current_player_it = players->erase(it);
  }
}

bool TurnManager::are_players_remaining() const { return !(players->empty()); }

Player& TurnManager::get_current_player() const { return current_player_it->second; }

float TurnManager::get_time_remaining() const { return time_remaining; }

float TurnManager::get_post_attack_grace_remaining() const {
  return post_attack_grace_remaining;
}
