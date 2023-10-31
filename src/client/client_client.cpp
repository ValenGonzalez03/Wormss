#include "client_client.h"

Client::Client(Socket&& skt) : prot(std::move(skt)), sender(prot), receiver(prot) {}

void Client::start_threads() {
    sender.start();
    receiver.start();
}

void Client::join_threads() {
    sender.join();
    receiver.join();
}