#include "client_sender_thread.h"

ClientSenderThread::ClientSenderThread(Protocol &protocol,
                                       Queue<std::shared_ptr<Command>> &send_queue)
    : prot(protocol), sender_queue(send_queue) {}

void ClientSenderThread::run() {
  // Blocking pop
    while (_keep_running) {
        try {
            // Obtengo el mensaje de la cola
            std::shared_ptr<Command> cmd = sender_queue.pop();
            // Envio el msj al server
            prot.send_command(*cmd);
        } catch (const std::exception& err) {
            //sender_queue.close();
            // Si la cola está cerrada o el socket está cerrado termino
            break;
        }
    }
    return;
}
