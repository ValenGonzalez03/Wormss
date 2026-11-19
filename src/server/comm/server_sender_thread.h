#ifndef SERVER_SENDER_THREAD_H
#define SERVER_SENDER_THREAD_H

#include "../../common/game_state.h"
#include "../../common/lib/queue.h"
#include "../../common/lib/thread.h"
#include "../world/server_world.h"
#include "server_protocol.h"
#include <map>
#include <string>

class ServerSender : public Thread {
 private:
  ServerProtocol &protocol;
  Queue<GameState> sender_queue;
  bool skt_closed = false;
  bool ended = false;
  uint8_t player_id;

 public:
  explicit ServerSender(ServerProtocol &protocol,  // NOLINT(runtime/references)
                        uint8_t player_id);

  // Envia al cliente su id y la lista de mundos disponibles para elegir. Devuelve el id del mundo elegido.
  uint8_t send_create_info(const uint8_t &player_id,
                           const std::map<uint8_t, std::string> &worlds_list);

  void send_game_started();

  // Envia el id del jugador al cliente.
  void send_id(const uint8_t id);

  // Envia una lista de mundos disponibles con sus nombres y sus ids al cliente.
  void send_worlds_list(const std::map<uint8_t, std::string> &world_names);

  // Envia toda la informacion necesaria del mundo al cliente.
  // (Nombre, background, spawn points, y características de las vigas)
  void send_world(const World *world);

  // Envia el estado final del jugador al cliente (ganador, perdedor o salida anticipada)
  void send_player_final_status(uint8_t final_status);

  // Devuelve true si el hilo Sender comenzó a ejecutarse, false en caso contrario.
  bool has_started();

  // Devuelve true si el hilo Sender terminó de ejecutarse, false en caso contrario.
  bool has_ended();

  // Devuelve true si el socket del cliente se cerró, false en caso contrario.
  bool is_closed();

  void close_queue();

  Queue<GameState> &get_queue();

  uint8_t get_player_id() const;

  // Ejecuta el loop del hilo Sender.
  void run() override;

  ~ServerSender();

  ServerSender(const ServerSender &) = delete;
  ServerSender &operator=(const ServerSender &) = delete;
};

#endif
