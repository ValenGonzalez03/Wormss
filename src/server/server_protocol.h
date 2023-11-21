#ifndef SERVER_PROTOCOL_H_
#define SERVER_PROTOCOL_H_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>

#include "../common/command.h"
#include "../common/game_state.h"
#include "../common/socket.h"
#include "../common/liberror.h"

#include "command_runnable_lobby.h"
#include "command_runnable_game.h"

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

  void send_game_state(GameState& game_state);

  void close_socket();
};

#endif
