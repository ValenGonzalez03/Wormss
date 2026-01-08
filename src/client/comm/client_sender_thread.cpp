#include "client_sender_thread.h"

ClientSenderThread::ClientSenderThread(ClientProtocol &protocol, Queue<std::shared_ptr<Command>> &send_queue, 
    bool& keep_playing)
    : prot(protocol), sender_queue(send_queue), keep_playing(keep_playing) {}

void ClientSenderThread::run() {
  // Blocking pop
    while (keep_playing && _keep_running) {
        try {
            // Obtengo el mensaje de la cola
            std::shared_ptr<Command> cmd = sender_queue.pop();
            // Envio el msj al server
            prot.send_command(*cmd);
        } catch (const std::exception& err) {
            //sender_queue.close(); // Cierro la cola para que no se queden esperando mensajes
            _keep_running = false;
            // Si la cola está cerrada o el socket está cerrado termino
            break;
        }
    }
    return;
}
