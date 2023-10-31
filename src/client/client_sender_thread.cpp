#include "client_sender_thread.h"

ClientSenderThread::ClientSenderThread(Protocol &protocol,
                                       Queue<Message> &send_queue)
    : prot(protocol), sender_queue(send_queue) {}

void ClientSenderThread::run() {
  // Blocking pop, non-blocking push
  // Pushea desde el rendererThread != main ?
  std::cout << "Sender Thread not implemented yet" << std::endl;
}
