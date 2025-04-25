#ifndef CLIENT_RECEIVER_THREAD_H_
#define CLIENT_RECEIVER_THREAD_H_

#include <string>

#include "../common/message.h"
#include "client_protocol.h"
#include "../common/lib/queue.h"
#include "../common/lib/thread.h"

class ClientReceiverThread : public Thread {
private:
  ClientProtocol &prot;
  Queue<GameState> &receiver_queue;

  // Elimino posibilidad de copias y operador =
  ClientReceiverThread(const ClientReceiverThread &) = delete;
  ClientReceiverThread &operator=(const ClientReceiverThread &) = delete;

public:
  explicit ClientReceiverThread(ClientProtocol &protocol,
                                Queue<GameState> &recv_queue);

  virtual void run() override;
};

#endif
