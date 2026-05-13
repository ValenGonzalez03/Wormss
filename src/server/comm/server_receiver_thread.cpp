#include "server_receiver_thread.h"
#include <string>

#define QUEUE_MAX_SIZE 20

ServerReceiver::ServerReceiver(uint8_t client_id, ServerProtocol &protocol, bool &keep_playing, bool &in_game,
                               std::mutex &m, std::condition_variable &is_empty) :
    client_id(client_id), protocol(protocol), lobby_commands(QUEUE_MAX_SIZE), game_commands(nullptr),
    is_empty(is_empty), keep_playing(keep_playing), in_game(in_game), m(m) {}

void ServerReceiver::run() {
  bool was_closed = false;
  try {
    // Loop en el lobby
    while (keep_playing) {
      std::unique_lock<std::mutex> lck(m);
      if (!in_game) {
        lobby_command_ptr runnable_command = protocol.process_command_lobby(&was_closed);
        lobby_commands.try_push(runnable_command);
        is_empty.wait(lck);
      } else {
        break;
      }
    }
    if (!keep_playing) {
      throw std::runtime_error("El receiver del cliente " + std::to_string(client_id) +
                               " terminó antes de entrar al juego.");
    }
    // Loop en el juego
    while (true) {
      game_command_ptr runnable_command = protocol.process_command(&was_closed);
      game_commands->try_push(runnable_command);
    }
  } catch (const LibError &libError) {  // Si se cierra el skt
    keep_playing = false;
    std::string str("Se Cerro el Socket: ");
    str.append(libError.what() + '\n');
    std::cout << str;
  } catch (const std::runtime_error &runtimeError) {  // Si se procesa mal un cmd
    keep_playing = false;
    std::cerr << "Error en el Receiver: " << runtimeError.what() << std::endl;
  }
  auto str = "Receiver de cliente: " + std::to_string(client_id) + " termino.\n";
  std::cout << str;
}

void ServerReceiver::wait_for_client_ready() {
  bool was_closed = false;
  if (!protocol.recv_client_ready(&was_closed)) {
    std::cerr << "No se recibió el comando de cliente listo para comenzar el juego.\n";
  }
}

Queue<lobby_command_ptr> &ServerReceiver::get_lobby_commands_queue() { return lobby_commands; }

void ServerReceiver::set_game_commands_queue(Queue<game_command_ptr> *queue) { game_commands = queue; }

ServerReceiver::~ServerReceiver() { keep_playing = false; }
