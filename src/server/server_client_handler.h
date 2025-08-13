#ifndef SERVER_CLIENT_HANDLER_H
#define SERVER_CLIENT_HANDLER_H

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

class ClientHandler {
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

public:

  // Constructor de la clase.
  explicit ClientHandler(Socket &&peer, GamesHandler &games_handler, uint8_t client_id);

  // Ejecuta los hilos.
  void start();

  // Si siguen vivos, cierra los hilos.
  void kill();

  // Joinea los hilos y cierra el socket.
  void finish();

  //Devuelve true si sus hilos estan muertos. False en caso contrario.
  bool is_dead();

  void set_commands_queue_to_receiver(Queue<std::shared_ptr<RunnableCommandGame>> *commands_queue);

  void manage_create_game();

  void manage_join_game(uint8_t game_id);

  void manage_start_game();

  ClientHandler(const ClientHandler &) = delete;
  ClientHandler &operator=(const ClientHandler &) = delete;
};

#endif