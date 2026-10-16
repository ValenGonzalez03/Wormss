#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include <map>
#include <cstdint>
#include "server_player.h"

enum class TurnState { ACTIVE, POST_ATTACK_GRACE, FINISHED };

class TurnManager {
 private:
  std::map<uint8_t, Player> *players;
  std::map<uint8_t, Player>::iterator current_player_it;
  TurnState state = TurnState::ACTIVE;
  float time_remaining;
  float post_attack_grace_remaining = 0.0f;

  // Avanza al siguiente jugador en la rotación de turnos y reinicia el tiempo del turno.
  void next_turn();

  // Elimina al jugador de la rotación de turnos sin avanzar al siguiente jugador.
  void remove_player(const uint8_t player_id);

 public:
  explicit TurnManager(std::map<uint8_t, Player> *players);

  // Actualiza el tiempo restante del turno y verifica si se debe pasar al siguiente turno
  // en caso de que se haya terminado el tiempo o si el gusano actual ha muerto.
  void update(const float dt);

  // Avanza al siguiente jugador en la rotación de turnos, reinicia el tiempo del turno y avanza al siguiente
  // gusano del jugador actual.
  void advance_full_turn();

  // Asigna el primer jugador de la lista como el jugador actual en caso de que exista.
  void assign_first_player();

  // En respuesta a que el jugador ha atacado, define el estado del turno como POST_ATTACK_GRACE.
  void on_attack();

  // En respuesta a que el gusano ha recibido daño, define el estado del turno como terminado.
  void on_damage();

  // Devuelve true si el jugador ha terminado su turno debido a un evento que lo produzca
  // Como atacar o recibir daño. Devuelve false en caso contrario.
  bool ended_his_turn() const;

  // Recibe un player_id y devuelve true si es el turno de ese jugador y si se puede mover en el turno,
  // false en caso contrario.
  bool can_play_in_turn(uint8_t player_id) const;

  // Determina que jugador ha atacado durante el turno actual.
  // void notify_attack_this_turn();

  // Verifica si hay jugadores restantes en la rotación de turnos. Devuelve true si
  // hay jugadores, false si no hay.
  bool are_players_remaining() const;

  Player &get_current_player() const;
  float get_time_remaining() const;
  float get_post_attack_grace_remaining() const;
};

#endif
