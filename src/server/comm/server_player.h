#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "../../common/lib/socket.h"
#include <atomic>
#include <vector>
#include <memory>

// #include "server_receiver_thread.h"
// #include "player_sender_thread.h"
// #include "../../common/game_state.h"
#include "player_sender_thread.h"
#include "../game/server_game.h"

class Player {
 private:
  uint8_t player_id;
  Game* game = nullptr;
  bool has_game_assigned = false;

 public:
  explicit Player(uint8_t player_id);  // NOLINT(runtime/references)

  Game* create_game(uint8_t game_id, const World& world, const GameConfig& game_config,
                    PlayerSender& sender,            // NOLINT(runtime/references)
                    Queue<game_command_ptr>& receiver_queue);  // NOLINT(runtime/references)

  void join_game(Game* game, PlayerSender& sender);  // NOLINT(runtime/references)

  void start_game();

  World* get_world();

  uint8_t get_game_id() const;

  bool has_game_started();

  bool has_game_finished();

  uint8_t get_id() const;


  Player(const Player&) = delete;
};

#endif
