#ifndef CLIENT_PROTOCOL_H_
#define CLIENT_PROTOCOL_H_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>

#include "../common/command.h"
#include "../common/game_state.h"
#include "../common/liberror.h"
#include "../common/socket.h"

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

  int receive_id();

  void send_command(Command &cmd);

  GameState process_game_state();

  void close_socket();
};

#endif
