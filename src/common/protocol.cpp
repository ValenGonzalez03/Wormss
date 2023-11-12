#include "protocol.h"

// Algunos posibles codigos de acciones que puede realizar el cliente para que
// se envien por el protocolo
namespace CODE_PLAYER_COMM {
uint8_t START_MOVING = 0x05;
uint8_t STOP_MOVING = 0x06;
uint8_t JUMP = 0x07;
uint8_t BACK_JUMP = 0x08;
uint8_t START_SHOOTING = 0x09;
uint8_t STOP_SHOOTING = 0x0A;
uint8_t USE_WEAPON_TOOL = 0x0B; // Seria para armas o herramientas que solo
                                // requieren tocar una vez el input,
uint8_t CHAT = 0x0C; // como el bate, el teletransportador, o el ataque aereo.
} // namespace CODE_PLAYER_COMM

Protocol::Protocol(Socket &&socket) : skt(std::move(socket)) {}

void Protocol::send_command(Command &cmd) {
  bool was_closed = false;
  cmd.send(skt, &was_closed);
}

std::unique_ptr<Command> Protocol::process_command() {
  bool was_closed = false;
  uint8_t code;
  uint8_t client_id = 0;
  skt.recvall(&code, sizeof(code), &was_closed);
  if (code == CODE_PLAYER_COMM::START_MOVING) {
    return std::make_unique<StartMoving>(client_id, skt, &was_closed);
  } else if (code == CODE_PLAYER_COMM::STOP_MOVING) {
    return std::make_unique<StopMoving>(client_id, skt, &was_closed);
  } else {
    std::cout << "Error de comando" << std::endl;
    throw(-1);
  }
}

void Protocol::send_game_state(GameState game_state) {
  bool was_closed = false;
  game_state.serialize(skt, &was_closed);
}

GameState Protocol::process_game_state() {
  bool was_closed = false;
  uint8_t worms_amount = 0;
  skt.recvall(&worms_amount, sizeof(worms_amount), &was_closed);

  std::vector<char> buf(worms_amount * sizeof(Worm) + 1);
  memcpy(buf.data(), &worms_amount, sizeof(worms_amount));
  skt.recvall(&buf[1], worms_amount * sizeof(Worm), &was_closed);

  return GameState(skt, &was_closed, buf);
}

void Protocol::close_socket() {
  skt.shutdown(2);
  skt.close();
}