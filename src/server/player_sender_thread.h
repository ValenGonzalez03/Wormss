#ifndef PLAYER_SENDER_THREAD_H
#define PLAYER_SENDER_THREAD_H

#include "../common/game_state.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "../common/protocol.h"

class PlayerSender : public Thread {
private:
  Protocol &protocol;
  std::shared_ptr<Queue<GameState>> sender_queue;
  std::atomic<bool> &keep_playing;

public:
  /*
   * Constructor de la clase.
   * */
  explicit PlayerSender(Protocol &protocol,
                        std::shared_ptr<Queue<GameState>> sender_queue,
                        std::atomic<bool>& keep_playing);

  void send_player_id(const int player_id);

  /*
   * Ejecuta el loop del hilo Sender.
   * */
  virtual void run() override;

  ~PlayerSender();

  PlayerSender(const PlayerSender &) = delete;
  PlayerSender &operator=(const PlayerSender &) = delete;
};

#endif
