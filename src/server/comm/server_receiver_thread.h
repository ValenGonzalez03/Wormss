#ifndef SERVER_RECEIVER_THREAD_H
#define SERVER_RECEIVER_THREAD_H

#include "../../common/lib/socket.h"
#include "../../common/lib/thread.h"
#include "../runnable_commands/command_runnable_game.h"
#include "../runnable_commands/command_runnable_lobby.h"
#include "server_protocol.h"
#include <condition_variable>
#include <memory>

class ServerReceiver : public Thread {
 private:
  Socket &skt;
  ServerProtocol &protocol;
  uint8_t client_id;
  Queue<std::shared_ptr<RunnableCommandLobby>> &lobby_commands;
  Queue<std::shared_ptr<RunnableCommandGame>> *game_commands = nullptr;
  bool &keep_playing;
  bool &in_game;
  std::mutex &m;
  std::condition_variable &is_empty;

 public:
  /*
   * Constructor de la clase.
   * */
  explicit ServerReceiver(Socket &skt, ServerProtocol &protocol,                         // NOLINT(runtime/references)
                          Queue<std::shared_ptr<RunnableCommandLobby>> &lobby_commands,  // NOLINT(runtime/references)
                          bool &keep_playing,                                            // NOLINT(runtime/references)
                          bool &in_game, std::mutex &m,                                  // NOLINT(runtime/references)
                          std::condition_variable &is_empty,                             // NOLINT(runtime/references)
                          uint8_t client_id);

  /*
   * Ejecuta el loop del ClientHandler.
   * */
  void run() override;

  void set_commands_queue(Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue);

  ~ServerReceiver();

  ServerReceiver(const ServerReceiver &) = delete;
  ServerReceiver &operator=(const ServerReceiver &) = delete;
};

#endif
