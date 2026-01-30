#include "client_protocol.h"

#include "../../common/protocol_codes.h"
#include "../../common/game_constants.h"
#include <arpa/inet.h>

ClientProtocol::ClientProtocol(Socket &&socket) : skt(std::move(socket)) {}

void ClientProtocol::send_command(const Command &cmd) {
  bool was_closed = false;
  cmd.send(skt, &was_closed);
}

// GameState ClientProtocol::process_game_state() {
//   bool was_closed = false;
//   return GameState(*this, &was_closed);
// }

void ClientProtocol::send_client_ready() {
  bool was_closed = false;
  uint8_t code = CODE_PLAYER_COMM::CLIENT_READY;
  skt.sendall(&code, sizeof(code), &was_closed);
}

uint8_t ClientProtocol::recv_byte(bool *was_closed) {
  uint8_t b = 0;
  skt.recvall(&b, sizeof(b), was_closed);
  return b;
}

#define MAX_BUFFER 1000

std::string ClientProtocol::recv_string(bool *was_closed) {
  uint16_t string_length;
  skt.recvall(&string_length, sizeof(string_length), was_closed);
  uint16_t string_length_be = ntohs(string_length);
  char buffer[string_length_be + 1];  // NOLINT(runtime/arrays)
  skt.recvall(buffer, string_length_be, was_closed);
  buffer[string_length] = '\0';
  return std::string(buffer);
}

void ClientProtocol::send_string(const std::string &str, bool *was_closed) {
  uint16_t string_length = str.size();
  uint16_t string_length_be = ntohs(string_length);
  skt.sendall(&string_length_be, sizeof(string_length_be), was_closed);
  skt.sendall(str.c_str(), str.size(), was_closed);
}

float ClientProtocol::recv_float(bool *was_closed) {
  int16_t number;
  skt.recvall(&number, sizeof(number), was_closed);
  int16_t number_be = ntohs(number);
  float final_number = number_be / 100.0f;
  return final_number;
}

bool ClientProtocol::recv_bool(bool *was_closed) {
  bool b = false;
  skt.recvall(&b, sizeof(b), was_closed);
  return b;
}

//////////////////////////////////////////////////////////////////////
/////////// FUNCIONES DE RECEPCIÓN DE MUNDO POR SOCKET ///////////////
//////////////////////////////////////////////////////////////////////

std::map<uint8_t, std::string> ClientProtocol::recv_worlds_map(bool *was_closed) {
  std::map<uint8_t, std::string> worlds_map;

  uint16_t worlds_number;
  skt.recvall(&worlds_number, sizeof(worlds_number), was_closed);
  uint16_t worlds_number_be = ntohs(worlds_number);

  for (int i = 0; i < worlds_number_be; i++) {
    uint8_t id = recv_byte(was_closed);
    std::string name = recv_string(was_closed);
    worlds_map[id] = name;
  }
  return worlds_map;
}

void ClientProtocol::send_world_name_selected(const std::string &world_name, bool *was_closed) {
  send_string(world_name, was_closed);
}

void ClientProtocol::send_world_id(int world_id, bool *was_closed) {
  skt.sendall(&world_id, sizeof(world_id), was_closed);
}

int ClientProtocol::recv_beams_number(bool *was_closed) {
  uint16_t beams_number;
  skt.recvall(&beams_number, sizeof(beams_number), was_closed);
  uint16_t beams_number_be = ntohs(beams_number);
  return beams_number_be;
}

BeamAttr ClientProtocol::recv_beam(bool *was_closed) {
  float pos_x = recv_float(was_closed);
  float pos_y = recv_float(was_closed);
  float angle = recv_float(was_closed);
  float width = recv_float(was_closed);

  BeamAttr data{pos_x, pos_y, angle, width};
  return data;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void ClientProtocol::close_socket() {
  skt.shutdown(2);
  skt.close();
}
