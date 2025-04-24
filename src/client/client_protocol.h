#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>

#include "../common/commands/command.h"
#include "../common/game_state.h"
#include "../common/liberror.h"
#include "../common/socket.h"
#include "client_worldview.h"

class ClientProtocol {
private:
  Socket skt;

public:
  explicit ClientProtocol(Socket &&socket);

  // Elimino posibilidad de copias y operador =
  ClientProtocol(const ClientProtocol &) = delete;
  ClientProtocol &operator=(const ClientProtocol &) = delete;

  // Implemento move semantics
  ClientProtocol(ClientProtocol &&) = default;
  ClientProtocol &operator=(ClientProtocol &&) = default;

  uint8_t receive_id();

  void send_command(Command &cmd);

  GameState process_game_state();

  // Recibe una cadena por socket y la devuelve
  std::string recv_string(bool *was_closed);

  // Envía el tamaño de una cadena y luego la cadena
  void send_string(std::string str, bool *was_closed);

  // Recibe un 'float' por socket, lo transforma y
  // lo devuelve
  float recv_float(bool *was_closed);

  // Recibe la cantidad de nombres; los nombres de los mundos y los devuelve
  std::vector<std::string> recv_worlds_names(bool *was_closed);

  // Recibe el mundo y lo carga
  // nombre, background, vigas
  void recv_world(WorldView &world, bool *was_closed);

  // Recibe una viga y la agrega al mundo
  void recv_and_add_beam(WorldView &world, bool *was_closed);

  void recv_and_add_spawn_point(std::vector<std::vector<float>> spawn_points, bool *was_closed);

  // Envía el nombre del mundo elegido
  void send_world_name_selected(std::string &world_name, bool *was_closed);

  // Envía el world_id
  void send_world_id(int world_id, bool *was_closed);

  void close_socket();
};

#endif
