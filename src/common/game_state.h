#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <arpa/inet.h>
#include <cstdint>
#include <list>
#include <map>
#include <vector>

#include "position.h"
#include "socket.h"

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

struct Worm {
private:
  uint8_t player_id;
  Position pos;
  uint8_t direction;
  uint8_t state; // Si está corriendo, disparando, saltando, etc

public:
  explicit Worm() : player_id(-1), pos(0, 0), direction(RIGHT), state(0) {}

  explicit Worm(uint8_t id, Position pos, u_int8_t dir, uint8_t st)
      : player_id(id), pos(pos), direction(dir), state(st) {}

  explicit Worm(Socket &skt) : pos(0, 0) {
    bool was_closed = false;
    deserialize(skt, &was_closed);
  }

  // Recibe la pos, la direccion, el state, etc del gusano (Lado cliente)
  void deserialize(Socket &skt, bool *was_closed) {
    skt.recvall(&player_id, sizeof(player_id), was_closed);
    uint16_t pos_x;
    uint16_t pos_y;
    skt.recvall(&pos_x, sizeof(pos_x), was_closed);
    skt.recvall(&pos_y, sizeof(pos_y), was_closed);
    float final_pos_x = ntohs(pos_x) / 100.0;
    float final_pos_y = ntohs(pos_y) / 100.0;
    // std::cout << "final_pos_x: " << final_pos_x << std::endl;
    // std::cout << "final_pos_y: " << final_pos_y << std::endl;
    Position position(final_pos_x, final_pos_y);
    this->pos = position;
    skt.recvall(&(this->direction), sizeof(this->direction), was_closed);
    skt.recvall(&(this->state), sizeof(this->state), was_closed);
  }

  // Envia los datos del gusano (Lado servidor)
  void serialize(Socket &skt, bool *was_closed) {
    // Hago send del player_id
    skt.sendall(&player_id, sizeof(player_id), was_closed);
    // Hago send de la position
    uint16_t pos_x = uint(pos.get_position_x() * 100);
    uint16_t pos_y = uint(pos.get_position_y() * 100);
    uint16_t pos_x_be = htons(pos_x);
    uint16_t pos_y_be = htons(pos_y);
    skt.sendall(&pos_x_be, sizeof(pos_x_be), was_closed);
    skt.sendall(&pos_y_be, sizeof(pos_y_be), was_closed);
    // Hago send de la direccion
    skt.sendall(&(this->direction), sizeof(this->direction), was_closed);
    // Hago send del estado
    skt.sendall(&(this->state), sizeof(this->state), was_closed);
  }

  uint8_t get_player_id() { return player_id; }

  float get_pos_x() { return pos.get_position_x(); }

  float get_pos_y() { return pos.get_position_y(); }

  uint8_t get_direction() { return direction; }

  uint8_t get_state() { return state; }

  // Devuelve la pos del gusano. Uso const para evitar que sea modificada
  Position get_position() const { return pos; }
};

struct GameState {
private:
  std::map<uint8_t, Worm> worms_list;

public:
  GameState() : worms_list(std::map<uint8_t, Worm>()) {}

  explicit GameState(const std::map<uint8_t, Worm> &list) : worms_list(list) {}

  // Constructor que funciona como una deserializacion, recibe la tira de bytes
  // y devuelve un game state
  GameState(Socket &skt, bool *was_closed) : worms_list() {
    uint8_t worms_amount = 0;
    skt.recvall(&worms_amount, sizeof(worms_amount), was_closed);
    for (int i = 0; i < worms_amount; i++) {
      Worm worm(skt);
      worms_list.insert(std::pair<uint8_t, Worm>(worm.get_player_id(), worm));
    }
  }

  void serialize(Socket &skt, bool *was_closed) {
    uint8_t worms_amount = worms_list.size();
    skt.sendall(&worms_amount, sizeof(worms_amount), was_closed);
    for (auto &worm : worms_list) {
      worm.second.serialize(skt, was_closed);
    }
  }

  std::map<uint8_t, Worm> get_worms() { return worms_list; }

  void add_worm(uint8_t player_id, const float &pos_x, const float &pos_y,
                uint8_t dir, uint8_t state) {
    Worm worm(player_id, Position(pos_x, pos_y), dir, state);
    worms_list.insert(std::pair<uint8_t, Worm>(worm.get_player_id(), worm));
  }
};

#endif
