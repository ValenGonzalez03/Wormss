#include "server_receiver_thread.h"
#include <string>

ServerReceiver::ServerReceiver(Socket &skt, ServerProtocol &protocol,
                               Queue<std::shared_ptr<RunnableCommandLobby>> &lobby_commands, bool &keep_playing,
                               bool &in_game, std::mutex &m, std::condition_variable &is_empty, uint8_t client_id) :
    skt(skt), protocol(protocol), keep_playing(keep_playing), in_game(in_game), lobby_commands(lobby_commands), m(m),
    is_empty(is_empty), client_id(client_id) {}

void ServerReceiver::run() {
  bool was_closed = false;
  try {
    while (keep_playing) {
      std::unique_lock<std::mutex> lck(m);
      is_empty.wait(lck);
      if (!in_game) {
        std::shared_ptr<RunnableCommandLobby> runnable_command = protocol.process_command_lobby();
        lobby_commands.try_push(runnable_command);
      } else {
        std::shared_ptr<RunnableCommandGame> runnable_command = protocol.process_command();
        game_commands->try_push(runnable_command);
      }
    }
  } catch (const LibError &libError) {  // Si se cierra el skt
    keep_playing = false;
  } catch (const std::runtime_error &runtimeError) {  // Si se procesa mal un cmd
    keep_playing = false;
    std::cerr << "RuntimeError: " << runtimeError.what() << std::endl;
  }
  std::cout << "RECEIVER TERMINO ";
}

void ServerReceiver::set_commands_queue(Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue) {
  this->game_commands = commands_queue;
}

ServerReceiver::~ServerReceiver() { keep_playing = false; }
