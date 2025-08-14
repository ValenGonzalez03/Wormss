#ifndef CLIENT_MANAGER_THREAD_H
#define CLIENT_MANAGER_THREAD_H

#include "../common/lib/socket.h"
#include <atomic>
#include <vector>

#include "server_receiver_thread.h"
#include "player_sender_thread.h"
#include "../common/game_state.h"
#include "server_games_handler.h"
#include "server_player.h"

class ServerReceiver;
class Player;

void start();
void join();

class ClientManager : public Thread {
private:
  Socket skt;
  uint8_t client_id;
  std::shared_ptr<Queue<GameState>> sender_queue;
  Queue<std::shared_ptr<RunnableCommandLobby>> lobby_commands_queue;
  std::mutex m;
  std::condition_variable is_empty;
  GamesHandler &games_handler;
  bool keep_playing = true;
  bool in_game = false;
  ServerProtocol protocol;
  PlayerSender sender;
  std::unique_ptr<ServerReceiver> receiver;
  std::shared_ptr<Player> player;

  bool threads_have_finished();

  bool has_ended();
public:

  // Constructor de la clase.
  explicit ClientManager(Socket &&peer, GamesHandler &games_handler, uint8_t client_id);

  // Ejecuta los hilos.
  virtual void run() override;

  // Si siguen vivos, cierra los hilos.
  void kill();

  // Joinea los hilos y cierra el socket.
  void finish();

  //Devuelve true si sus hilos estan muertos. False en caso contrario.
  bool is_dead();

  void set_commands_queue_to_receiver(Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue);

  ClientManager(const ClientManager &) = delete;
  ClientManager &operator=(const ClientManager &) = delete;

};

#endif