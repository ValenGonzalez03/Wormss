#include "client_receiver_thread.h"

ClientReceiverThread::ClientReceiverThread(Protocol &protocol,
                                           Queue<GameState> &recv_queue)
    : prot(protocol), receiver_queue(recv_queue) {}

void ClientReceiverThread::run() {
  // Blocking push
  while (_keep_running) {
  std::cout << "Receiver Thread not implemented yet" << std::endl;
    try {
      GameState game_state = prot.process_game_state();
      receiver_queue.push(game_state);

    } catch (const std::exception &e) {
      //receiver_queue.close();
      _keep_running = false;
      std::cout << "Socket cerrado" << std::endl;
      break;
    }
  }
}
