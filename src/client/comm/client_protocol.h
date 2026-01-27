#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include <cstdint>
#include <string>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>
#include <map>

#include "../../common/game_constants.h"
#include "../../common/commands/command.h"
#include "../../common/lib/liberror.h"
#include "../../common/lib/socket.h"

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

  uint8_t recv_byte(bool *was_closed);

  void send_command(const Command &cmd);

  // GameState process_game_state();

  void send_client_ready();

  // Recibe una cadena por socket y la devuelve
  std::string recv_string(bool *was_closed);

  // Envía el tamaño de una cadena y luego la cadena
  void send_string(std::string str, bool *was_closed);

  // Recibe un 'float' por socket, lo transforma y
  // lo devuelve
  float recv_float(bool *was_closed);

  bool recv_bool(bool *was_closed);

  ////////////////////////////////////////////////////////////////////////
  /////////////// FUNCIONES DE RECEPCIÓN DE MUNDO POR SOCKET ////////////

  // Recibe la cantidad y los nombres de los mundos y los retorna.
  std::map<uint8_t, std::string> recv_worlds_map(bool *was_closed);

  // Envía el nombre 'world_name' del mundo elegido.
  void send_world_name_selected(const std::string &world_name, bool *was_closed);

  // Envía el 'world_id'.
  void send_world_id(int world_id, bool *was_closed);

  // Recibe el numero de vigas en el mundo y las retorna.
  int recv_beams_number(bool *was_closed);

  // Recibe los datos de una viga y los retorna.
  BeamAttr recv_beam(bool *was_closed);

  /////////////// FUNCIONES DE RECEPCIÓN DE MUNDO POR SOCKET ///////////////
  /////////////////////////////////////////////////////////////////////////

  void close_socket();
};

#endif
