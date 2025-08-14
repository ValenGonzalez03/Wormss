#include "player_sender_thread.h"
#include <arpa/inet.h>

PlayerSender::PlayerSender(ServerProtocol &protocol,
                           std::shared_ptr<Queue<GameState>> sender_queue,
                           bool &keep_playing)
    : protocol(protocol), sender_queue(sender_queue),
      keep_playing(keep_playing) {}

void PlayerSender::run() {
  bool was_closed = false;
  try {
    while (keep_playing) {
      GameState game_state;
      if (sender_queue->try_pop(game_state)) {
        protocol.send_game_state(game_state);
      }
    }
  } catch (const std::exception &err) {
  }
  std::cout << "SENDER TERMINO ";
}

void PlayerSender::send_id(const uint8_t id) {
  protocol.send_id(id);
}

void PlayerSender::send_worlds_names(const std::vector<std::string>& world_names) {
  bool was_closed = false;
  protocol.send_worlds_names(world_names, &was_closed);
}

void PlayerSender::send_world(World& world) {
  bool was_closed = false;
  protocol.send_world(world);
}

bool PlayerSender::has_started() {
  return is_alive();
}

PlayerSender::~PlayerSender() {
  keep_playing = false;
}
