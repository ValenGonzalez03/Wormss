#ifndef SERVER_ACCEPT_THREAD_H
#define SERVER_ACCEPT_THREAD_H

#include "../common/socket.h"
#include "../common/thread.h"
#include "server_games_handler.h"
#include "server_player.h"
#include <list>

class Accept : public Thread {
private:
  Socket skt;
  std::atomic<bool> is_alive{true};
  std::list<std::shared_ptr<Player>> players;
  GamesHandler games_handler;
  uint8_t id_counter = 1;

  /*
   * Busca todos los players muertos en la lista de players y los joinea.
   */
  void reap_dead();

  /*
   * Recorre la lista de players y mata todos los players aun vivos.
   */
  void kill_all();

public:
  /*
   * Constructor de la clase.
   * */
  explicit Accept(Socket &skt);

  /*
   * Ejecuta el hilo Aceptador.
   * */
  void run() override;

  /*
   * Cierra el socket y detiene la espera de nuevos clientes.
   */
  void kill();

  Accept(const Accept &) = delete;
  Accept &operator=(const Accept &) = delete;
};

#endif
