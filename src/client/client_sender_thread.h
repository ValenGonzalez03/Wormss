#ifndef CLIENT_SENDER_THREAD_H_
#define CLIENT_SENDER_THREAD_H_

#include <string>

#include "../common/message.h"
#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/thread.h"

class ClientSenderThread : public Thread {
private:
  Protocol &prot;
  Queue<Message> &sender_queue;

  // Elimino posibilidad de copias y operador =
  ClientSenderThread(const ClientSenderThread &) = delete;
  ClientSenderThread &operator=(const ClientSenderThread &) = delete;

public:
  explicit ClientSenderThread(
      Protocol &protocol,
      Queue<Message> &send_queue); //, Queue<Message>& queue);

  virtual void run() override;
};

#endif
