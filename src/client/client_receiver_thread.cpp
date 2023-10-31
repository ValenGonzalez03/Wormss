#include "client_receiver_thread.h"

ClientReceiverThread::ClientReceiverThread(Protocol& protocol) : 
                prot(protocol),
                receiver_queue() {}

void ClientReceiverThread::run() {
    // Blocking push, non-blocking pop
    // Popea desde el rendererThread != main ?
    std::cout << "Receiver Thread not implemented yet" << std::endl;
}
