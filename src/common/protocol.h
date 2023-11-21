#ifndef COMMON_PROTOCOL_H_
#define COMMON_PROTOCOL_H_

#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <utility>

#include "command.h"
#include "game_state.h"
#include "socket.h"
#include "liberror.h"
#include "create_game.h"
#include "join_game.h"
#include "start_game.h"
#include "start_moving.h"
#include "stop_moving.h"

class Protocol {
private:
  Socket skt;

public:
  explicit Protocol(Socket &&socket);

  // Elimino posibilidad de copias y operador =
  Protocol(const Protocol &) = delete;
  Protocol &operator=(const Protocol &) = delete;

  // Implemento move semantics
  Protocol(Protocol &&) = default;
  Protocol &operator=(Protocol &&) = default;

  void send_command(Command &cmd);

  std::shared_ptr<Command> process_command();

  std::shared_ptr<Command> process_command_lobby();

  void send_game_state(GameState& game_state);

  GameState process_game_state();

  void close_socket();
};

#endif
