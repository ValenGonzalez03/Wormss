#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "../common/lib/socket.h"
#include <atomic>
#include <vector>

#include "client_handler_thread.h"
#include "player_sender_thread.h"
#include "../common/game_state.h"
#include "server_games_handler.h"

class Player {
private:
  Socket skt;
  std::shared_ptr<Queue<GameState>> sender_queue;
  GamesHandler &games_handler;
  std::atomic<bool> keep_playing{true};
  std::atomic<bool> in_game{false};
  ServerProtocol protocol;
  PlayerSender sender;
  uint8_t player_id;
  ClientHandler client_handler;

public:
  /*
   * Constructor de la clase.
   * */
  explicit Player(Socket &&peer, GamesHandler &games_handler,
                  std::shared_ptr<Queue<GameState>> sender_queue,
                  uint8_t player_id);

  /*
   * Ejecuta los hilos.
   * */
  void start();

  /*
   * Si siguen vivos, cierra los hilos.
   * */
  void kill();

  /*
   * Joinea los hilos.
   * */
  void join();

  /*
   * Devuelve true si sus hilos estan muertos. False en caso contrario.
   * */
  bool is_dead();

  Player(const Player &) = delete;
  Player &operator=(const Player &) = delete;
};

#endif
