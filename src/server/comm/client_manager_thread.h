#ifndef CLIENT_MANAGER_THREAD_H
#define CLIENT_MANAGER_THREAD_H

#include "../../common/lib/socket.h"
#include <atomic>
#include <vector>
#include <memory>

#include "server_receiver_thread.h"
#include "player_sender_thread.h"
#include "../../common/game_state.h"
#include "../game/server_games_handler.h"
#include "server_player.h"

class Player;

void start();
void join();

class ClientManager : public Thread {
 private:
  uint8_t client_id;
  GamesHandler &games_handler;
  Player player;
  bool is_host = false;

  ServerProtocol protocol;
  PlayerSender sender;
  ServerReceiver receiver;
  Queue<lobby_command_ptr> &lobby_commands_queue;

  std::mutex m;
  std::condition_variable is_empty;
  bool keep_playing = true;
  bool in_game = false;
  bool in_game_lobby = false;

  bool threads_have_finished();

  // Devuelve true si el cliente terminó el juego.
  bool has_ended();

 public:
  // Constructor de la clase.
  explicit ClientManager(Socket &&peer, GamesHandler &games_handler,  // NOLINT(runtime/references)
                         uint8_t client_id);

  // Ejecuta los hilos.
  void run() override;

  void manage_create_game();

  void manage_join_game(const uint8_t &game_id);

  void manage_start_game(const uint8_t &game_id);

  void manage_game_started();

  void set_to_host();

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
