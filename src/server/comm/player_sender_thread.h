#ifndef PLAYER_SENDER_THREAD_H
#define PLAYER_SENDER_THREAD_H

#include "../../common/game_state.h"
#include "../../common/lib/queue.h"
#include "../../common/lib/socket.h"
#include "../../common/lib/thread.h"
#include "../world/server_world.h"
#include "server_protocol.h"

class PlayerSender : public Thread {
private:
  ServerProtocol &protocol;
  std::shared_ptr<Queue<GameState>> sender_queue;
  bool &keep_playing;

public:
  /*
   * Constructor de la clase.
   * */
  explicit PlayerSender(ServerProtocol &protocol,
                        std::shared_ptr<Queue<GameState>> sender_queue,
                        bool &keep_playing);

  void send_id(const uint8_t id);

  void send_worlds_names(const std::vector<std::string>& world_names);

  void send_beams_number();

  void send_world(World& world);

  bool has_started();

  /*
   * Ejecuta el loop del hilo Sender.
   * */
  virtual void run() override;

  ~PlayerSender();

  PlayerSender(const PlayerSender &) = delete;
  PlayerSender &operator=(const PlayerSender &) = delete;
};

#endif
