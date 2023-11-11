#include "client_sender_thread.h"

ClientSenderThread::ClientSenderThread(Protocol &protocol,
                                       Queue<std::shared_ptr<Command>> &send_queue)
    : prot(protocol), sender_queue(send_queue) {}

void ClientSenderThread::run() {
  // Blocking pop
    bool was_closed = false;
    while (!was_closed && _keep_running) {
        try {
            // Obtengo el mensaje de la cola
            std::shared_ptr<Command> cmd = sender_queue.pop();
            // Envio el msj al server
            prot.send_command(*cmd);
            if (was_closed) {
                std::cout << "Socket cerrado" << std::endl;
                break;
            }
        } catch (const std::exception& err) {
            // Si la cola está cerrada termino
            break;
        }
    }
    return;
}
