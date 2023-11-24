#include "client_protocol.h"

#include "../common/protocol_codes.h"


ClientProtocol::ClientProtocol(Socket &&socket) : skt(std::move(socket)) {}

void ClientProtocol::send_command(Command &cmd) {
  bool was_closed = false;
  cmd.send(skt, &was_closed);
}

GameState ClientProtocol::process_game_state() {
  bool was_closed = false;
  return GameState(skt, &was_closed);
}

std::string ClientProtocol::recv_string(bool* was_closed) {
  uint16_t string_length;
  skt.recvall(&string_length, sizeof(string_length), was_closed);
  uint16_t string_length_be = ntohs(string_length);
  char buffer[string_length_be + 1]; //CORREGIR?
  skt.recvall(buffer, sizeof(string_length_be), was_closed);
  buffer[string_length] = '\0';
  return std::string(buffer);
}

float ClientProtocol::recv_float(bool* was_closed) {
  uint16_t number;
  skt.recvall(&number, sizeof(number), was_closed);
  uint16_t number_be = ntohs(number);
  float final_number = number_be * 100;
  return final_number;
}

//////////////////////////////////////////////////////////////////////
///////////FUNCIONES DE RECEPCIÓN DE MUNDO POR SOCKET/////////////////
//////////////////////////////////////////////////////////////////////

std::vector<std::string> ClientProtocol::recv_worlds_names(bool* was_closed) {
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


void ClientProtocol::recv_world(WorldView& world, bool* was_closed) {
  std::string world_name = recv_string(was_closed);
  std::string backgorund_path = recv_string(was_closed);
  
  // Recibo cant de vigas y caracteristicas vigas
  uint16_t beams_number;
  skt.recvall(&beams_number, sizeof(beams_number), was_closed);
  uint16_t beams_number_be = ntohs(beams_number);
  for (int i = 0; i < beams_number_be; i++) {
    recv_and_add_beam(world, was_closed);
  }
  // Hace falta recibir spawn_points?
}


void ClientProtocol::recv_and_add_beam(WorldView& world, bool* was_closed) {
  uint16_t beam_angle;
  
  float pos_x = recv_float(was_closed);
  float pos_y = recv_float(was_closed);
  skt.recvall(&beam_angle, sizeof(beam_angle), was_closed);
  
  uint16_t beam_angle_be = ntohs(beam_angle);
  int beam_angle_int = static_cast<int>(beam_angle_be);
  
  float width = recv_float(was_closed);

  if (width == 6) {
    world.add_long_beam(pos_x, pos_y, beam_angle_int);
  }
  else if (width == 3) {
    world.add_short_beam(pos_x, pos_y, beam_angle_int);
  }
  else {
    std::cout << "Error tamanio viga" << std::endl;
  }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


void ClientProtocol::close_socket() {
  skt.shutdown(2);
  skt.close();
}