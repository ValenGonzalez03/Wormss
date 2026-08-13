#include "server_sender_thread.h"
#include <arpa/inet.h>
#include <list>

#define QUEUE_MAX_SIZE 20

ServerSender::ServerSender(ServerProtocol &protocol, uint8_t player_id) :
    protocol(protocol), sender_queue(QUEUE_MAX_SIZE), player_id(player_id) {}

void ServerSender::run() {
  bool was_closed = false;
  try {
    while (!was_closed) {
      GameState game_state;
      if (sender_queue.try_pop(game_state))
        game_state.serialize(protocol, &was_closed);
    }
    // GameState game_state;
    // while (sender_queue.try_pop(game_state)) {
    //   game_state.serialize(protocol, &was_closed);
    //   // protocol.send_game_state(game_state);
    // }
  } catch (const LibError &liberr) {
    closed = true;
    auto str = "[SENDER-THREAD]: Conexión con cliente finalizada en cliente: " +
               std::to_string(player_id) + '\n';
    std::cout << str;
  } catch (const std::exception &err) {
    std::cerr << "[SENDER-THREAD]: Error en el sender: " << err.what() << "\n";
  }
  auto str =
      "[SENDER-THREAD]: Sender de cliente: " + std::to_string(player_id) + " termino.\n";
  std::cout << str;
}

uint8_t ServerSender::send_create_info(
    const uint8_t &player_id, const std::map<uint8_t, std::string> &worlds_list) {
  send_id(player_id);
  send_worlds_list(worlds_list);

  bool was_closed = false;
  return protocol.recv_world_id(&was_closed);
}

void ServerSender::send_game_started() {
  bool was_closed = false;
  protocol.send_game_started(&was_closed);
}

void ServerSender::send_id(const uint8_t id) {
  bool was_closed = false;
  protocol.send_byte(id, &was_closed);
}

void ServerSender::send_worlds_list(const std::map<uint8_t, std::string> &worlds_list) {
  bool was_closed = false;
  protocol.send_worlds_list(worlds_list, &was_closed);
}

void ServerSender::send_world(const World *world) {
  bool was_closed = false;
  protocol.send_string(world->get_name(), &was_closed);
  protocol.send_string(world->get_background(), &was_closed);

  std::list<BeamBody *> beams = world->get_beams();
  uint8_t beams_number = beams.size();
  protocol.send_byte(beams_number, &was_closed);

  for (auto &beam : beams) {
    BeamAttr attr = beam->get_attr();
    protocol.send_beam(attr, &was_closed);
  }
}

bool ServerSender::has_started() { return is_alive(); }

bool ServerSender::is_closed() { return closed; }

Queue<GameState> &ServerSender::get_queue() { return sender_queue; }

ServerSender::~ServerSender() {}
