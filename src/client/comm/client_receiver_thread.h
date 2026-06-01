#ifndef CLIENT_RECEIVER_THREAD_H_
#define CLIENT_RECEIVER_THREAD_H_

#include <string>

#include "../../common/message.h"
#include "client_protocol.h"
#include "../../common/game_state.h"
#include "../../common/lib/queue.h"
#include "../../common/lib/thread.h"

class ClientReceiverThread : public Thread {
 private:
  ClientProtocol &prot;
  Queue<GameState> &receiver_queue;
  bool &keep_playing;

  // Elimino posibilidad de copias y operador =
  ClientReceiverThread(const ClientReceiverThread &) = delete;
  ClientReceiverThread &operator=(const ClientReceiverThread &) = delete;

 public:
  explicit ClientReceiverThread(
      ClientProtocol &protocol,      // NOLINT(runtime/references)
      Queue<GameState> &recv_queue,  // NOLINT(runtime/references)
      bool &keep_playing);           // NOLINT(runtime/references)

  void run() override;
};

#endif
