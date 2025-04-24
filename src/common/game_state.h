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

struct WormData {
private:
  uint8_t player_id;
  float pos_x; // En metros
  float pos_y; // En metros
  //Position pos;
  uint8_t direction;
  uint8_t state; // Si está corriendo, disparando, saltando, etc
  float aim_angle;
  // uint16_t id;

public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  explicit WormData()
      : player_id(-1), pos_x(0), pos_y(0), direction(RIGHT), state(0), aim_angle(0) {}

  explicit WormData(uint8_t id, float pos_x, float pos_y, u_int8_t dir, uint8_t st, float angle)
      : player_id(id), pos_x(pos_x), pos_y(pos_y), direction(dir), state(st), aim_angle(angle) {}

  explicit WormData(Socket &skt) : pos_x(0), pos_y(0) {
    bool was_closed = false;
    deserialize(skt, &was_closed);
  }

  // Recibe la pos, la direccion, el state, etc del gusano (Lado cliente)
  void deserialize(Socket &skt, bool *was_closed) {
    // Recibo el player_id
    skt.recvall(&player_id, sizeof(player_id), was_closed);
    // Recibo la position
    uint16_t pos_x;
    uint16_t pos_y;
    skt.recvall(&pos_x, sizeof(pos_x), was_closed);
    skt.recvall(&pos_y, sizeof(pos_y), was_closed);
    float final_pos_x = ntohs(pos_x) / 100.0;
    float final_pos_y = ntohs(pos_y) / 100.0;
    // std::cout << "final_pos_x: " << final_pos_x << std::endl;
    // std::cout << "final_pos_y: " << final_pos_y << std::endl;
    //Position position(final_pos_x, final_pos_y);
    this->pos_x = final_pos_x;
    this->pos_y = final_pos_y;
    // Recibo la direccion
    skt.recvall(&(this->direction), sizeof(this->direction), was_closed);
    // Recibo el estado
    skt.recvall(&(this->state), sizeof(this->state), was_closed);
    // Recibo el angulo de apuntado
    int angle_int_net;
    skt.recvall(&angle_int_net, sizeof(angle_int_net), was_closed);
    int angle_int = ntohl(angle_int_net);
    this->aim_angle = float(angle_int) / float(100.0);
  }

  // Envia los datos del gusano (Lado servidor)
  void serialize(Socket &skt, bool *was_closed) {
    // Hago send del player_id
    skt.sendall(&(this->player_id), sizeof(this->player_id), was_closed);
    // Hago send de la position
    // std::cout << "final_pos_x: " << pos_x << std::endl;
    // std::cout << "final_pos_y: " << pos_y << std::endl;
    uint16_t pos_x = uint(this->pos_x * 100);
    uint16_t pos_y = uint(this->pos_y * 100);
    uint16_t pos_x_be = htons(pos_x);
    uint16_t pos_y_be = htons(pos_y);
    skt.sendall(&pos_x_be, sizeof(pos_x_be), was_closed);
    skt.sendall(&pos_y_be, sizeof(pos_y_be), was_closed);
    // Hago send de la direccion
    skt.sendall(&(this->direction), sizeof(this->direction), was_closed);
    // Hago send del estado
    skt.sendall(&(this->state), sizeof(this->state), was_closed);
    // Hago send del angulo de apuntado
    int angle_int = int(this->aim_angle * 100);
    int angle_int_net = htonl(angle_int);
    skt.sendall(&angle_int_net, sizeof(angle_int_net), was_closed);
  }

  uint8_t get_player_id() { return player_id; }

  float get_pos_x() { return pos_x; }

  float get_pos_y() { return pos_y; }

  uint8_t get_direction() { return direction; }

  uint8_t get_state() { return state; }

  float get_aim_angle() { return aim_angle; }

  // Devuelve la pos del gusano. Uso const para evitar que sea modificada
  //Position get_position() const { return pos; }
};

struct GameState {
private:
  std::map<uint8_t, WormData> worms_list;

public:
  GameState() : worms_list(std::map<uint8_t, WormData>()) {}

  explicit GameState(const std::map<uint8_t, WormData> &list) : worms_list(list) {}

  // Constructor que funciona como una deserializacion, recibe la tira de bytes
  // y devuelve un game state
  GameState(Socket &skt, bool *was_closed) : worms_list() {
    uint8_t worms_amount = 0;
    skt.recvall(&worms_amount, sizeof(worms_amount), was_closed);
    for (int i = 0; i < worms_amount; i++) {
      WormData worm(skt);
      worms_list.insert(std::pair<uint8_t, WormData>(worm.get_player_id(), worm));
      //std::cout << "worms_list_size: " << (int) worms_list.size() << std::endl;
    }
  }

  void serialize(Socket &skt, bool *was_closed) {
    uint8_t worms_amount = worms_list.size();
    //std::cout << "Cantidad gusanos: " << worms_amount << std::endl;
    skt.sendall(&worms_amount, sizeof(worms_amount), was_closed);
    for (auto &worm : worms_list) {
      worm.second.serialize(skt, was_closed);
    }
  }

  std::map<uint8_t, WormData> get_worms() { return worms_list; }

  void add_worm(uint8_t player_id, const float &pos_x, const float &pos_y,
                uint8_t dir, uint8_t state, float angle) {
    WormData worm(player_id, pos_x, pos_y, dir, state, angle);
    worms_list.insert(std::pair<uint8_t, WormData>(worm.get_player_id(), worm));
  }
};

#endif
