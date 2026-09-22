#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <list>
#include <cstdint>
#include "../bodies/worm_body.h"

class Player {
 private:
  uint8_t player_id;
  std::map<uint8_t, WormBody *> worms = {};
  std::list<uint8_t> worm_ids = {};
  std::map<uint8_t, WormBody *>::iterator current_worm_it;

 public:
  explicit Player(uint8_t player_id);

  // Añade worm a la lista de gusanos del jugador y si es el primer gusano,
  // lo establece como el gusano actual.
  void add_worm(WormBody *worm);

  // Elimina el gusano con el id especificado de la lista de gusanos del jugador.
  void remove_worm(uint8_t worm_id);

  // Avanza al siguiente gusano vivo en la rotación de gusanos.
  void advance_worm();

  // Devuelve true si el jugador no tiene gusanos vivos, false en caso contrario.
  bool has_lost() const;

  uint8_t get_player_id() const;
  WormBody *get_current_worm() const;
};

#endif
