#ifndef CLIENT_RECEIVER_THREAD_H_
#define CLIENT_RECEIVER_THREAD_H_

#include <string>

#include "../common/message.h"
#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/thread.h"

class ClientReceiverThread : public Thread {
private:
  Protocol &prot;
  Queue<GameState> &receiver_queue;

  // Elimino posibilidad de copias y operador =
  ClientReceiverThread(const ClientReceiverThread &) = delete;
  ClientReceiverThread &operator=(const ClientReceiverThread &) = delete;

public:
  explicit ClientReceiverThread(Protocol &protocol,
                                Queue<GameState> &recv_queue);

  virtual void run() override;
};

#endif
