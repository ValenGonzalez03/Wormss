#ifndef CLIENT_SENDER_THREAD_H_
#define CLIENT_SENDER_THREAD_H_

#include <string>

#include "../common/message.h"
#include "client_protocol.h"
#include "../common/lib/queue.h"
#include "../common/lib/thread.h"

class ClientSenderThread : public Thread {
private:
  ClientProtocol &prot;
  Queue<std::shared_ptr<Command>> &sender_queue;
  bool& keep_playing;

  // Elimino posibilidad de copias y operador =
  ClientSenderThread(const ClientSenderThread &) = delete;
  ClientSenderThread &operator=(const ClientSenderThread &) = delete;

public:
  explicit ClientSenderThread(ClientProtocol &protocol, Queue<std::shared_ptr<Command>> &send_queue, 
                              bool& keep_playing);

  virtual void run() override;

};

#endif
