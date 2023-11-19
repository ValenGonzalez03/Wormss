#ifndef CLIENT_STATE_H_
#define CLIENT_STATE_H_

#include "../common/game_state.h"

struct client_state {
  bool is_running = false; // whether the character is currently running
  int run_phase = -1;      // run animation phase
  float position_x = 0.0;  // player position
  int direction = 0;
  // cppcheck-suppress unusedStructMember
  unsigned int prev_ticks;
  GameState last_game_state = GameState();
};

#endif