#include <cstdint>
#ifndef COMMON_PROTOCOL_CODES_H
#define COMMON_PROTOCOL_CODES_H

// Algunos posibles codigos de acciones que puede realizar el cliente para que
// se envien por el protocolo
namespace CODE_PLAYER_COMM {
uint8_t CREATE_GAME = 0x01;
uint8_t JOIN_GAME = 0x02;
uint8_t START_GAME = 0x03;
uint8_t GAME_STARTED = 0x04;
uint8_t START_MOVING = 0x05;
uint8_t STOP_MOVING = 0x06;
uint8_t JUMP = 0x07;
uint8_t BACK_JUMP = 0x08;
uint8_t START_AIMING = 0x09;
uint8_t STOP_AIMING = 0x0A;
uint8_t CHARGING_ATTACK = 0x0B;  // Para Bazooka y Granada
uint8_t ATTACKING = 0x0C;        // Para todas las armas
uint8_t CHANGE_WEAPON = 0x0D;
uint8_t CLIENT_READY = 0x0E;
uint8_t STOP_GAME = 0x0F;
uint8_t HOST_STARTED_GAME = 0x10;
}  // namespace CODE_PLAYER_COMM

#endif
