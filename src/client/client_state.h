#ifndef CLIENT_STATE_H_
#define CLIENT_STATE_H_

#include "../common/game_state.h"

struct client_state {
  bool is_running = false; // whether the character is currently running
  bool is_jumping = false;
  int run_phase = 0;      // run animation phase
  int jump_phase = 0;
  float position_x = 0.0;  // player position
  int direction = 0;
  // cppcheck-suppress unusedStructMember
  unsigned int prev_ticks;
  GameState last_game_state = GameState();
};

#endif