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

void ClientProtocol::close_socket() {
  skt.shutdown(2);
  skt.close();
}