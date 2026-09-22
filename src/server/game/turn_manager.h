#ifndef TURN_MANAGER_H
#define TURN_MANAGER_H

#include <map>
#include <cstdint>
#include "server_player.h"

class TurnManager {
 private:
  std::map<uint8_t, Player> *players;
  std::map<uint8_t, Player>::iterator current_player_it;
  float time_remaining;

  // Avanza al siguiente jugador en la rotación de turnos y reinicia el tiempo del turno.
  void next_turn();

  // Elimina al jugador de la rotación de turnos sin avanzar al siguiente jugador.
  void remove_player(const uint8_t player_id);

 public:
  explicit TurnManager(std::map<uint8_t, Player> *players);

  // Actualiza el tiempo restante del turno y verifica si se debe pasar al siguiente turno
  // en caso de que se haya terminado el tiempo o si el gusano actual ha muerto.
  void update(const float dt);

  // Asigna el primer jugador de la lista como el jugador actual en caso de que exista.
  void assign_first_player();

  // Recibe un player_id y devuelve true si es el turno de ese jugador,
  // false en caso contrario.
  bool is_turn_of(uint8_t player_id) const;

  // Verifica si hay jugadores restantes en la rotación de turnos. Devuelve true si
  // hay jugadores, false si no hay.
  bool are_players_remaining() const;

  Player &get_current_player() const;
  float get_time_remaining() const;
};

#endif
