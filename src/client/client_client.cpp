#include "client_client.h"

Client::Client(Socket &&skt)
    : prot(std::move(skt)), receiver_queue(), sender_queue(),
      receiver(prot, receiver_queue), sender(prot, sender_queue) {}

void Client::start_threads() {
  sender.start();
  receiver.start();
}

void Client::join_threads() {
  sender.join();
  receiver.join();
}
