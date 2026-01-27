#include "client_receiver_thread.h"

ClientReceiverThread::ClientReceiverThread(ClientProtocol &protocol, Queue<GameState> &recv_queue, bool &keep_playing) :
    prot(protocol), receiver_queue(recv_queue), keep_playing(keep_playing) {}

void ClientReceiverThread::run() {
  // Blocking push
  while (keep_playing && _keep_running) {
    try {
      bool was_closed = false;
      GameState game_state(prot, &was_closed);
      // GameState game_state = prot.process_game_state();
      receiver_queue.push(game_state);
    } catch (const std::exception &e) {
      // receiver_queue.close();
      _keep_running = false;
      break;
    }
  }
}
