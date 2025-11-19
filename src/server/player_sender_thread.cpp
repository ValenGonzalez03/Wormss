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
        game_state.serialize(protocol, &was_closed);
        //protocol.send_game_state(game_state);
      }
    }
    GameState game_state;
    while (sender_queue->try_pop(game_state)) {
      game_state.serialize(protocol, &was_closed);
      //protocol.send_game_state(game_state);
    }
  } catch (const std::exception &err) {
  }
  std::cout << "SENDER TERMINO ";
}

void PlayerSender::send_id(const uint8_t id) {
  bool was_closed = false;
  protocol.send_byte(id, &was_closed);
}

void PlayerSender::send_worlds_names(const std::vector<std::string>& world_names) {
  bool was_closed = false;
  protocol.send_worlds_names(world_names, &was_closed);
}

void PlayerSender::send_world(World& world) {
  bool was_closed = false;
  protocol.send_string(world.get_name(), &was_closed);
  protocol.send_string(world.get_background(), &was_closed);

  std::list<BeamBody*> beams = world.get_beams();
  uint8_t beams_number = beams.size();
  protocol.send_byte(beams_number, &was_closed);

  for (auto &beam : beams) {
    BeamAttr attr = beam->get_attr();
    protocol.send_beam(attr, &was_closed);
  }
  //protocol.send_world(world);
}

bool PlayerSender::has_started() {
  return is_alive();
}

PlayerSender::~PlayerSender() {
  keep_playing = false;
}
