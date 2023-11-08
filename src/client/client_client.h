#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <SDL2pp/SDL2pp.hh>
#include <string>
#include <utility>

#include "../common/protocol.h"
#include "../common/queue.h"
#include "../common/socket.h"
#include "client_position.h"
#include "client_receiver_thread.h"
#include "client_resource_pool.h"
#include "client_sender_thread.h"
#include "client_worldview.h"

#define LEFT 0
#define RIGHT 1

class Client {
private:
  Protocol prot;
  Queue<Message> receiver_queue;
  Queue<std::shared_ptr<Command>> sender_queue;
  // Hice que la clase Client tenga las queues porque el cliente tambien va a
  // hacer pops y pushs para mandarselas al game loop. Al sender y al receiver
  // les pase referencias de las queues. Podria cambiarse, no se si deberia
  // tenerla Client.
  ClientReceiverThread receiver;
  ClientSenderThread sender;
  std::map<int, std::function<void()>> controls;

public:
  explicit Client(Socket &&skt);

  void start_threads();

  void join_threads();

  int run();

  void handle_start_moving(int direction, bool &is_running);

  void handle_stop_moving(bool &is_running);
};

#endif
