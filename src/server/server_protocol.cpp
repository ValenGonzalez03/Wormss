#include "server_protocol.h"

#include "../common/protocol_codes.h"

#include "create_game_runnable.h"
#include "join_game_runnable.h"
#include "start_game_runnable.h"
#include "start_moving_runnable.h"
#include "stop_moving_runnable.h"

ServerProtocol::ServerProtocol(Socket &&socket) : skt(std::move(socket)) {}

void ServerProtocol::send_id(const int id) {
  bool was_closed = false;
  skt.sendall(&id, sizeof(id), &was_closed);
}

std::shared_ptr<RunnableCommandGame> ServerProtocol::process_command() {
  bool was_closed = false;
  uint8_t code;
  uint8_t client_id = 0;
  skt.recvall(&code, sizeof(code), &was_closed);

  if (was_closed) {
    throw LibError(errno, "Socket is closed.");
  }

  if (code == CODE_PLAYER_COMM::START_MOVING) {
    return std::make_shared<RunnableStartMoving>(client_id, skt, &was_closed);
  } else if (code == CODE_PLAYER_COMM::STOP_MOVING) {
    return std::make_shared<RunnableStopMoving>(client_id, skt, &was_closed);
  } else {
    throw std::runtime_error("Error de comando de juego");
  }
}

std::shared_ptr<RunnableCommandLobby> ServerProtocol::process_command_lobby() {
  bool was_closed = false;
  uint8_t code;
  uint8_t client_id = 0;
  skt.recvall(&code, sizeof(code), &was_closed);

  if (was_closed) {
    throw LibError(errno, "Socket is closed.");
  }

  if (code == CODE_PLAYER_COMM::CREATE_GAME) {
    return std::make_shared<RunnableCreateGame>(client_id, skt, &was_closed);
  } else if (code == CODE_PLAYER_COMM::JOIN_GAME) {
    return std::make_shared<RunnableJoinGame>(client_id, skt, &was_closed);
  } else if (code == CODE_PLAYER_COMM::START_GAME) {
    return std::make_shared<RunnableStartGame>(client_id, skt, &was_closed);
  } else {
    throw std::runtime_error("Error de comando de lobby");
  }
}

void ServerProtocol::send_game_state(GameState &game_state) {
  bool was_closed = false;
  game_state.serialize(skt, &was_closed);
}

void ServerProtocol::send_id(const int id) {
  bool was_closed = false;
  skt.sendall(&id, sizeof(id), &was_closed);
}

void ServerProtocol::close_socket() {
  skt.shutdown(2);
  skt.close();
}