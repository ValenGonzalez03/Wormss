#ifndef CLIENT_MANAGER_THREAD_H
#define CLIENT_MANAGER_THREAD_H

#include "../../common/lib/socket.h"
#include <atomic>
#include <vector>
#include <memory>

#include "server_receiver.h"
#include "server_sender_thread.h"
#include "../../common/game_state.h"
#include "../game/server_games_handler.h"

// class Player;

void start();
void join();

class ClientManager : public Thread {
 private:
  uint8_t client_id;
  GamesHandler &games_handler;
  GameManager *game = nullptr;
  // Player player;

  ServerProtocol protocol;
  ServerSender sender;
  ServerReceiver receiver;
  Queue<lobby_command_ptr> &lobby_commands_queue;

  std::mutex m;
  bool keep_playing = true;
  bool in_game = false;
  bool in_game_lobby = false;

 public:
  // Constructor de la clase.
  explicit ClientManager(Socket &&peer,
                         GamesHandler &games_handler,  // NOLINT(runtime/references)
                         uint8_t client_id);

  // Ejecuta los hilos.
  void run() override;

  void manage_create_game();

  void manage_join_game(const uint8_t &game_id);

  void manage_start_game(const uint8_t &game_id);  // Para el host de la partida

  void manage_game_started();  // Para los que se unieron

  // Si siguen vivos, cierra los hilos.
  void kill();

  // Joinea los hilos y cierra el socket.
  void finish(bool was_closed);

  // Devuelve true si sus hilos estan muertos. False en caso contrario.
  bool is_dead();

  ClientManager(const ClientManager &) = delete;
  ClientManager &operator=(const ClientManager &) = delete;
};

#endif
