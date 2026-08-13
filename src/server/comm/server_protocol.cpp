#include "server_protocol.h"

#include "../../common/protocol_codes.h"

#include "../runnable_commands/create_game_runnable.h"
#include "../runnable_commands/join_game_runnable.h"
#include "../runnable_commands/game_started_runnable.h"
#include "../runnable_commands/jump_runnable.h"
#include "../runnable_commands/start_aiming_runnable.h"
#include "../runnable_commands/start_game_runnable.h"
#include "../runnable_commands/start_moving_runnable.h"
#include "../runnable_commands/stop_aiming_runnable.h"
#include "../runnable_commands/stop_moving_runnable.h"
#include "../runnable_commands/start_shooting_runnable.h"
#include "../runnable_commands/stop_game_runnable.h"
#include "../runnable_commands/change_weapon_runnable.h"

class RunnableCommandLobby;

ServerProtocol::ServerProtocol(Socket &&socket) : skt(std::move(socket)) {}

game_command_ptr ServerProtocol::process_command(bool *was_closed) {
  uint8_t code;
  uint8_t client_id = 0;
  skt.recvall(&client_id, sizeof(client_id), was_closed);
  skt.recvall(&code, sizeof(code), was_closed);

  if (code == CODE_PLAYER_COMM::START_MOVING) {
    return std::make_shared<RunnableStartMoving>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::STOP_MOVING) {
    return std::make_shared<RunnableStopMoving>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::JUMP) {
    return std::make_shared<RunnableJump>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::START_AIMING) {
    return std::make_shared<RunnableStartAiming>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::STOP_AIMING) {
    return std::make_shared<RunnableStopAiming>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::CHANGE_WEAPON) {
    return std::make_shared<RunnableChangeWeapon>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::START_SHOOTING) {
    return std::make_shared<RunnableStartShooting>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::STOP_GAME) {
    return std::make_shared<RunnableStopGame>(client_id, skt, was_closed);
  } else {
    if (was_closed)
      return nullptr;
    throw std::runtime_error("Comando de juego desconocido.");
  }
}

lobby_command_ptr ServerProtocol::process_command_lobby(bool *was_closed) {
  uint8_t code;
  uint8_t client_id = 0;
  skt.recvall(&code, sizeof(code), was_closed);
  // std::cout << "Codigo leido: " << (int)code << std::endl;

  if (code == CODE_PLAYER_COMM::CREATE_GAME) {
    return std::make_shared<RunnableCreateGame>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::JOIN_GAME) {
    return std::make_shared<RunnableJoinGame>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::START_GAME) {
    return std::make_shared<RunnableStartGame>(client_id, skt, was_closed);
  } else if (code == CODE_PLAYER_COMM::GAME_STARTED) {
    return std::make_shared<RunnableGameStarted>(client_id, skt, was_closed);
  } else {
    if (was_closed)
      return nullptr;
    throw std::runtime_error("Comando de lobby desconocido.");
  }
}

void ServerProtocol::close_socket(bool was_closed) {
  if (!was_closed) {
    skt.shutdown(2);
    skt.close();
  }
}

void ServerProtocol::send_game_started(bool *was_closed) {
  uint8_t code = CODE_PLAYER_COMM::HOST_STARTED_GAME;
  skt.sendall(&code, sizeof(code), was_closed);
}

bool ServerProtocol::recv_client_ready(bool *was_closed) {
  uint8_t code;
  skt.recvall(&code, sizeof(code), was_closed);
  return (code == CODE_PLAYER_COMM::CLIENT_READY);
}

int ServerProtocol::recv_world_id(bool *was_closed) {
  int id = 0;
  skt.recvall(&id, sizeof(id), was_closed);
  return id;
}

void ServerProtocol::send_string(const std::string &str, bool *was_closed) {
  uint16_t string_length = str.size();
  uint16_t string_length_be = ntohs(string_length);
  skt.sendall(&string_length_be, sizeof(string_length_be), was_closed);
  skt.sendall(str.c_str(), str.size(), was_closed);
}

#define MAX_BUFFER 1000

std::string ServerProtocol::recv_string(bool *was_closed) {
  uint16_t string_length;
  skt.recvall(&string_length, sizeof(string_length), was_closed);
  uint16_t string_length_be = ntohs(string_length);
  std::cout << "str_length: " << string_length_be << std::endl;
  char buffer[string_length_be + 1];  // NOLINT(runtime/arrays)
  skt.recvall(buffer, string_length_be, was_closed);
  buffer[string_length] = '\0';
  return std::string(buffer);
}

void ServerProtocol::send_byte(const uint8_t n, bool *was_closed) {
  skt.sendall(&n, sizeof(n), was_closed);
}

void ServerProtocol::send_float(float n, bool *was_closed) {
  int16_t number = static_cast<int>(n * 100);
  int16_t number_be = htons(number);
  skt.sendall(&number_be, sizeof(number_be), was_closed);
}

void ServerProtocol::send_bool(bool b, bool *was_closed) {
  skt.sendall(&b, sizeof(b), was_closed);
}


//////////////////////////////////////////////////////////////////////
/////////// FUNCIONES DE ENVÍO DE MUNDO POR SOCKET //////////////////

void ServerProtocol::send_beam(BeamAttr beam_attr, bool *was_closed) {
  send_float(beam_attr.pos_x, was_closed);
  send_float(beam_attr.pos_y, was_closed);
  send_float(beam_attr.angle, was_closed);
  send_float(beam_attr.width, was_closed);
}

void ServerProtocol::send_spawn_points(const std::vector<float> &spawn_point,
                                       bool *was_closed) {
  send_float(spawn_point[0], was_closed);
  send_float(spawn_point[1], was_closed);
}

void ServerProtocol::send_worlds_list(const std::map<uint8_t, std::string> &worlds_list,
                                      bool *was_closed) {
  uint16_t worlds_number = worlds_list.size();
  uint16_t worlds_number_be = htons(worlds_number);
  skt.sendall(&worlds_number_be, sizeof(worlds_number_be), was_closed);
  for (const auto &pair : worlds_list) {
    send_byte(pair.first, was_closed);     // Envio id del mundo
    send_string(pair.second, was_closed);  // Envio nombre del mundo
  }
}

/////////// FUNCIONES DE ENVÍO DE MUNDO POR SOCKET ////////////////////
//////////////////////////////////////////////////////////////////////
