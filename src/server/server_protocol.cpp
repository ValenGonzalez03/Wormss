#include "server_protocol.h"

#include "../common/protocol_codes.h"

#include "runnable_commands/create_game_runnable.h"
#include "runnable_commands/join_game_runnable.h"
#include "runnable_commands/jump_runnable.h"
#include "runnable_commands/start_aiming_runnable.h"
#include "runnable_commands/start_game_runnable.h"
#include "runnable_commands/start_moving_runnable.h"
#include "runnable_commands/stop_aiming_runnable.h"
#include "runnable_commands/stop_moving_runnable.h"

ServerProtocol::ServerProtocol(Socket &&socket) : skt(std::move(socket)) {}

std::shared_ptr<RunnableCommandGame>
ServerProtocol::process_command(const uint8_t &client_id) {
  bool was_closed = false;
  uint8_t code;
  skt.recvall(&code, sizeof(code), &was_closed);

  if (was_closed) {
    throw LibError(errno, "Socket is closed.");
  }

  if (code == CODE_PLAYER_COMM::START_MOVING) {
    return std::make_shared<RunnableStartMoving>(client_id, skt, &was_closed);
  } else if (code == CODE_PLAYER_COMM::STOP_MOVING) {
    return std::make_shared<RunnableStopMoving>(client_id, skt, &was_closed);
  } else if (code == CODE_PLAYER_COMM::JUMP) {
    return std::make_shared<RunnableJump>(client_id, skt, &was_closed);
  } else if (code == CODE_PLAYER_COMM::START_AIMING) {
    return std::make_shared<RunnableStartAiming>(client_id, skt, &was_closed);
  } else if (code == CODE_PLAYER_COMM::STOP_AIMING) {
    return std::make_shared<RunnableStopAiming>(client_id, skt, &was_closed);
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

void ServerProtocol::send_id(const uint8_t id) {
  bool was_closed = false;
  skt.sendall(&id, sizeof(id), &was_closed);
}

void ServerProtocol::close_socket() {
  skt.shutdown(2);
  skt.close();
}

int ServerProtocol::recv_world_id(bool* was_closed) {
  int id = 0;
  skt.recvall(&id, sizeof(id), was_closed);
  return id;
}

void ServerProtocol::send_string(std::string str, bool *was_closed) {
  uint16_t string_length = str.size();
  uint16_t string_length_be = ntohs(string_length);
  skt.sendall(&string_length_be, sizeof(string_length_be), was_closed);
  skt.sendall(str.c_str(), str.size(), was_closed);
}

std::string ServerProtocol::recv_string(bool* was_closed) {
  uint16_t string_length;
  skt.recvall(&string_length, sizeof(string_length), was_closed);
  uint16_t string_length_be = ntohs(string_length);
  std::cout << "str_length: " << string_length_be << std::endl;
  char buffer[string_length_be + 1];
  skt.recvall(buffer, string_length_be, was_closed);
  buffer[string_length] = '\0';
  return std::string(buffer);
}

void ServerProtocol::send_float(float n, bool *was_closed) {
  uint16_t number = uint(n * 100);
  uint16_t number_be = htons(number);
  skt.sendall(&number_be, sizeof(number_be), was_closed);
}


//////////////////////////////////////////////////////////////////////
///////////FUNCIONES DE ENVÍO DE MUNDO POR SOCKET/////////////////////
//////////////////////////////////////////////////////////////////////
void ServerProtocol::send_world(World &world) {
  bool was_closed = false;
  // Envio el nombre del mundo
  send_string(world.get_name(), &was_closed);

  // Envio el path del background del mundo
  send_string(world.get_background(), &was_closed);

  // Envio la cantidad de vigas y sus posiciones
  uint16_t beams_number = world.get_beams().size();
  uint16_t beams_number_be = htons(beams_number);
  skt.sendall(&beams_number_be, sizeof(beams_number_be), &was_closed);
  for (auto &beam : world.get_beams()) {
    send_beam(*beam, &was_closed);
  }
  // Envio la cantidad de spawn points y sus posiciones
  // uint16_t spawn_points_number = world.get_spawn_points().size();
  // uint16_t spawn_points_number_be = htons(spawn_points_number);
  // skt.sendall(&spawn_points_number, sizeof(spawn_points_number), &was_closed);
  // for (auto spawn_point : world.get_spawn_points()) {
  //   send_spawn_points(spawn_point, &was_closed);
  // }
}

void ServerProtocol::send_beam(BeamBody &beam, bool *was_closed) {
  int16_t beam_angle = static_cast<int16_t>(beam.get_angle());
  int16_t beam_angle_be = htons(beam_angle);
  send_float(beam.get_pos_x(), was_closed);
  send_float(beam.get_pos_y(), was_closed);
  skt.sendall(&beam_angle_be, sizeof(beam_angle_be), was_closed);
  send_float(beam.get_width(), was_closed);
}

void ServerProtocol::send_spawn_points(std::vector<float> spawn_point, bool *was_closed) {
  send_float(spawn_point[0], was_closed);
  send_float(spawn_point[1], was_closed);
}

void ServerProtocol::send_worlds_names(
    const std::vector<std::string>& world_names, bool *was_closed) {
  uint16_t worlds_number = world_names.size();
  uint16_t worlds_number_be = htons(worlds_number);
  skt.sendall(&worlds_number_be, sizeof(worlds_number_be), was_closed);
  for (auto &world_name : world_names) {
    send_string(world_name, was_closed);
  }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
