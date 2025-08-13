#ifndef SERVER_PLAYER_H
#define SERVER_PLAYER_H

#include "../common/lib/socket.h"
#include <atomic>
#include <vector>

#include "server_receiver_thread.h"
#include "player_sender_thread.h"
#include "../common/game_state.h"
#include "server_games_handler.h"

class Player {
private:
  uint8_t player_id;
  Game* game;
  GamesHandler& games_handler;
  PlayerSender& sender;
  std::shared_ptr<Queue<GameState>> sender_queue;
  ServerProtocol& protocol;
  bool has_game_assigned = false;

public:
  
  //explicit Player();
  /*
   * Constructor de la clase.
   * */
  explicit Player(uint8_t player_id, Game* game, GamesHandler& games_handler, PlayerSender& sender, std::shared_ptr<Queue<GameState>> sender_queue, ServerProtocol& protocol);

  void initialize_game();

  uint8_t get_game_id() const;

  bool has_game_started();

  bool has_game_finished();
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

  Queue<std::shared_ptr<RunnableCommandGame>>& get_commands_queue_game();

  void manage_create_game();

  void manage_join_game(uint8_t game_id);

  void manage_start_game();

  Player(const Player &) = delete;
};

#endif
