#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>

#include "../common/commands/command.h"
#include "../common/game_state.h"
#include "../common/liberror.h"
#include "../common/socket.h"

#include "runnable_commands/command_runnable_game.h"
#include "runnable_commands/command_runnable_lobby.h"

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

  std::shared_ptr<RunnableCommandGame> process_command();

  std::shared_ptr<RunnableCommandLobby> process_command_lobby();

  void send_game_state(GameState &game_state);

  void send_id(const uint8_t id);

  // Envía el tamaño de una cadena y luego la cadena
  void send_string(std::string str, bool *was_closed);

  // Multiplica un número flotante por 100 para mandarlo
  // como uint por el socket, al recibirlo hay que
  // dividirlo por 100 para obtener otra vez el num original
  void send_float(float n, bool *was_closed);

  //////////////////////////////////////////////////////////////////////
  ///////////FUNCIONES DE ENVÍO DE MUNDO POR SOCKET/////////////////////
  //////////////////////////////////////////////////////////////////////

  // Envía el mundo pasado por parámetro por el socket
  // (nombre, background path, vigas, spawn_points)
  void send_world(World &world);

  // Envía las características de una viga
  // (pos_x, pos_y, angle, width)
  void send_beam(BeamBody &beam, bool *was_closed);

  // Envía las carac de un spawn_point del mundo
  void send_spawn_point(WormBody &worm, bool *was_closed);

  // Recibe una lista de punteros a mundos y envía la cantidad de
  // mundos y sus nombres por socket
  void send_worlds_names(std::vector<std::shared_ptr<World>> &worlds,
                         bool *was_closed);

  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////

  void close_socket();
};

#endif
