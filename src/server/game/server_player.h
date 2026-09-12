#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <cstdint>
#include "../bodies/worm_body.h"

class Player {
 private:
  uint8_t player_id;
  std::map<uint8_t, WormBody *> worms = {};
  std::map<uint8_t, WormBody *>::iterator current_worm_it;

 public:
  explicit Player(uint8_t player_id);

  void add_worm(WormBody *worm);
  void remove_worm(WormBody *worm);
  void advance_worm();  // rota al siguiente gusano vivo al final del turno

  bool has_lost() const;

  uint8_t get_player_id() const;
  WormBody *get_current_worm() const;
};

#endif
