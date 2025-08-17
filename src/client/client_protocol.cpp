#include "client_protocol.h"

#include "../common/protocol_codes.h"

ClientProtocol::ClientProtocol(Socket &&socket) : skt(std::move(socket)) {}

uint8_t ClientProtocol::receive_id() {
  bool was_closed = false;
  uint8_t id = 0;
  // esto habria que chequearlo, no se si funcionaria bien
  skt.recvall(&id, sizeof(id), &was_closed);
  return id;
}

void ClientProtocol::send_command(Command &cmd) {
  bool was_closed = false;
  cmd.send(skt, &was_closed);
}

GameState ClientProtocol::process_game_state() {
  bool was_closed = false;
  return GameState(skt, &was_closed);
}

void ClientProtocol::send_client_ready() {
  bool was_closed = false;
  uint8_t code = CODE_PLAYER_COMM::CLIENT_READY;
  skt.sendall(&code, sizeof(code), &was_closed);
}

std::string ClientProtocol::recv_string(bool *was_closed) {
  uint16_t string_length;
  skt.recvall(&string_length, sizeof(string_length), was_closed);
  uint16_t string_length_be = ntohs(string_length);
  char buffer[string_length_be + 1];
  skt.recvall(buffer, string_length_be, was_closed);
  buffer[string_length] = '\0';
  return std::string(buffer);
}

void ClientProtocol::send_string(std::string str, bool *was_closed) {
  uint16_t string_length = str.size();
  uint16_t string_length_be = ntohs(string_length);
  skt.sendall(&string_length_be, sizeof(string_length_be), was_closed);
  skt.sendall(str.c_str(), str.size(), was_closed);
}

float ClientProtocol::recv_float(bool *was_closed) {
  uint16_t number;
  skt.recvall(&number, sizeof(number), was_closed);
  uint16_t number_be = ntohs(number);
  float final_number = number_be / 100;
  return final_number;
}

//////////////////////////////////////////////////////////////////////
///////////FUNCIONES DE RECEPCIÓN DE MUNDO POR SOCKET/////////////////
//////////////////////////////////////////////////////////////////////

std::vector<std::string> ClientProtocol::recv_worlds_names(bool *was_closed) {
  std::vector<std::string> names;
  uint16_t names_number;
  skt.recvall(&names_number, sizeof(names_number), was_closed);
  uint16_t names_number_be = ntohs(names_number);
  for (int i = 0; i < names_number_be; i++) {
    std::string name = recv_string(was_closed);
    names.push_back(name);
  }
  return names;
}

void ClientProtocol::send_world_name_selected(std::string &world_name,
                                              bool *was_closed) {
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
  uint16_t beam_angle;

  float pos_x = recv_float(was_closed);
  float pos_y = recv_float(was_closed);
  skt.recvall(&beam_angle, sizeof(beam_angle), was_closed);

  uint16_t beam_angle_be = ntohs(beam_angle);
  int beam_angle_int = static_cast<int>(beam_angle_be);

  float width = recv_float(was_closed);

  BeamAttr data {pos_x, pos_y, beam_angle_int, width};
  return data;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void ClientProtocol::close_socket() {
  skt.shutdown(2);
  skt.close();
}