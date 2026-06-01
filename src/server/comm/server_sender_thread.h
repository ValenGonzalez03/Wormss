#ifndef SERVER_SENDER_THREAD_H
#define SERVER_SENDER_THREAD_H

#include "../../common/game_state.h"
#include "../../common/lib/queue.h"
#include "../../common/lib/socket.h"
#include "../../common/lib/thread.h"
#include "../world/server_world.h"
#include "server_protocol.h"
#include <map>
#include <memory>
#include <string>

class ServerSender : public Thread {
 private:
  ServerProtocol &protocol;
  Queue<GameState> sender_queue;
  // bool &keep_playing;
  bool closed = false;
  uint8_t player_id;

 public:
  explicit ServerSender(ServerProtocol &protocol,  // NOLINT(runtime/references)
                        uint8_t player_id);

  // Envia al cliente su id y la lista de mundos disponibles para elegir. Devuelve el id del mundo elegido.
  uint8_t send_create_info(const uint8_t &player_id,
                           const std::map<uint8_t, std::string> &worlds_map);

  void send_game_started();

  void send_id(const uint8_t id);

  void send_worlds_map(const std::map<uint8_t, std::string> &world_names);

  void send_beams_number();

  void send_world(const World *world);

  bool has_started();

  bool is_closed();

  Queue<GameState> &get_queue();

  // Ejecuta el loop del hilo Sender.
  void run() override;

  ~ServerSender();

  ServerSender(const ServerSender &) = delete;
  ServerSender &operator=(const ServerSender &) = delete;
};

#endif
