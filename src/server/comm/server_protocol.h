#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>
#include <map>
#include <vector>
#include <string>

#include "../../common/commands/command.h"
#include "../../common/game_constants.h"
#include "../../common/lib/liberror.h"
#include "../../common/lib/socket.h"

#include "../runnable_commands/command_runnable_lobby.h"
#include "../runnable_commands/command_runnable_game.h"

class RunnableCommandLobby;

class ServerProtocol {
 private:
  Socket skt;

 public:
  explicit ServerProtocol(Socket &&socket);

  // Elimino posibilidad de copias y operador =
  ServerProtocol(const ServerProtocol &) = delete;
  ServerProtocol &operator=(const ServerProtocol &) = delete;

  // Implemento move semantics
  ServerProtocol(ServerProtocol &&) = default;
  ServerProtocol &operator=(ServerProtocol &&) = default;

  game_command_ptr process_command(bool *was_closed);

  lobby_command_ptr process_command_lobby(bool *was_closed);

  void send_game_started(bool *was_closed);

  bool recv_client_ready(bool *was_closed);

  int recv_world_id(bool *was_closed);

  std::string recv_string(bool *was_closed);

  void send_byte(const uint8_t id, bool *was_closed);

  // Envía el tamaño de una cadena y luego la cadena
  void send_string(const std::string &str, bool *was_closed);

  // Multiplica un número flotante por 100 para mandarlo como uint por el socket, al recibirlo hay que
  // dividirlo por 100 para obtener otra vez el num original
  void send_float(float n, bool *was_closed);

  void send_bool(bool b, bool *was_closed);

  //////////////////////////////////////////////////////////////////////
  /////////// FUNCIONES DE ENVÍO DE MUNDO POR SOCKET /////////////////////

  // Envía las características de una viga (pos_x, pos_y, angle, width)
  void send_beam(BeamAttr beam_attr, bool *was_closed);

  // Envía las carac de un spawn_point del mundo
  void send_spawn_points(const std::vector<float> &spawn_point, bool *was_closed);

  // Recibe una lista de punteros a mundos y envía la cantidad de mundos y sus nombres por socket
  void send_worlds_map(const std::map<uint8_t, std::string> &worlds_map, bool *was_closed);

  /////////// FUNCIONES DE ENVÍO DE MUNDO POR SOCKET /////////////////////
  //////////////////////////////////////////////////////////////////////

  void close_socket();
};

#endif
