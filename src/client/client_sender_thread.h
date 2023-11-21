#ifndef CLIENT_SENDER_THREAD_H_
#define CLIENT_SENDER_THREAD_H_

#include <string>

#include "../common/message.h"
#include "client_protocol.h"
#include "../common/queue.h"
#include "../common/thread.h"

class ClientSenderThread : public Thread {
private:
  ClientProtocol &prot;
  Queue<std::shared_ptr<Command>> &sender_queue;

  // Elimino posibilidad de copias y operador =
  ClientSenderThread(const ClientSenderThread &) = delete;
  ClientSenderThread &operator=(const ClientSenderThread &) = delete;

public:
  explicit ClientSenderThread(
      ClientProtocol &protocol,
      Queue<std::shared_ptr<Command>> &send_queue); //, Queue<Message>& queue);

  virtual void run() override;
};

#endif
