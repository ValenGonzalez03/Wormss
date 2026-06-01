#ifndef SERVER_RECEIVER_H
#define SERVER_RECEIVER_H

#include "../../common/lib/socket.h"
#include "../../common/lib/thread.h"
#include "../runnable_commands/command_runnable_game.h"
#include "../runnable_commands/command_runnable_lobby.h"
#include "server_protocol.h"
#include <condition_variable>
#include <memory>

class ServerReceiver {
 private:
  uint8_t client_id;
  ServerProtocol &protocol;
  Queue<lobby_command_ptr> lobby_commands;
  Queue<game_command_ptr> *game_commands;

  // std::condition_variable &is_empty;
  // bool &keep_playing;
  bool &in_game;
  // std::mutex &m;

 public:
  explicit ServerReceiver(uint8_t client_id,
                          ServerProtocol &protocol,  // NOLINT(runtime/references)
                          bool &in_game);            // NOLINT(runtime/references));

  // Ejecuta el loop del Receiver esperando comandos de lobby y posteriormente de juego.
  void run();

  // Espera hasta recibir el comando de cliente listo para comenzar el juego.
  void wait_for_client_ready();

  Queue<lobby_command_ptr> &get_lobby_commands_queue();

  void set_game_commands_queue(Queue<game_command_ptr> *queue);

  ~ServerReceiver();

  ServerReceiver(const ServerReceiver &) = delete;
  ServerReceiver &operator=(const ServerReceiver &) = delete;
};

#endif
