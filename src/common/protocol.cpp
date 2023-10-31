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
