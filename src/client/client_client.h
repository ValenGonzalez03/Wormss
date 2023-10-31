#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "client_receiver_thread.h"
#include "client_sender_thread.h"
#include <string>
#include <utility>

class Client {
private:
  Protocol prot;
  Queue<Message> receiver_queue;
  Queue<Message> sender_queue;
  // Hice que la clase Client tenga las queues porque el cliente tambien va a
  // hacer pops y pushs para mandarselas al game loop. Al sender y al receiver
  // les pase referencias de las queues. Podria cambiarse, no se si deberia
  // tenerla Client.
  ClientReceiverThread receiver;
  ClientSenderThread sender;

public:
  explicit Client(Socket &&skt);

  void start_threads();

  void join_threads();
};

#endif
