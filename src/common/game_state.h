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
  float aim_angle;
  // uint16_t id;

public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  explicit Worm()
      : player_id(-1), pos(0, 0), direction(RIGHT), state(0), aim_angle(0) {}

  explicit Worm(uint8_t id, Position pos, u_int8_t dir, uint8_t st, float angle)
      : player_id(id), pos(pos), direction(dir), state(st), aim_angle(angle) {}

  explicit Worm(Socket &skt) : pos(0, 0) {
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
    Position position(final_pos_x, final_pos_y);
    this->pos = position;
    // Recibo la direccion
    skt.recvall(&(this->direction), sizeof(this->direction), was_closed);
    // Recibo el estado
    skt.recvall(&(this->state), sizeof(this->state), was_closed);
    // Recibo el angulo de apuntado
    int angle_int;
    skt.recvall(&angle_int, sizeof(angle_int), was_closed);
    this->aim_angle = ntohl(angle_int) / 100.0;
  }

  // Envia los datos del gusano (Lado servidor)
  void serialize(Socket &skt, bool *was_closed) {
    // Hago send del player_id
    skt.sendall(&(this->player_id), sizeof(this->player_id), was_closed);
    // Hago send de la position
    uint16_t pos_x = uint(pos.get_position_x() * 100);
    uint16_t pos_y = uint(pos.get_position_y() * 100);
    uint16_t pos_x_be = htons(pos_x);
    uint16_t pos_y_be = htons(pos_y);
    // std::cout << "final_pos_x: " << pos.get_position_x() << std::endl;
    // std::cout << "final_pos_y: " << pos.get_position_y() << std::endl;
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

  float get_pos_x() { return pos.get_position_x(); }

  float get_pos_y() { return pos.get_position_y(); }

  uint8_t get_direction() { return direction; }

  uint8_t get_state() { return state; }

  float get_aim_angle() { return aim_angle; }

  // Devuelve la pos del gusano. Uso const para evitar que sea modificada
  Position get_position() const { return pos; }
};

struct GameState {
private:
  std::map<uint8_t, Worm> worms_list;
  // uint16_t players_amount;

public:
  GameState() : worms_list(std::map<uint8_t, Worm>()) {}

  explicit GameState(const std::map<uint8_t, Worm> &list) : worms_list(list) {}

  // Constructor que funciona como una deserializacion, recibe la tira de bytes
  // y devuelve un game state
  GameState(Socket &skt, bool *was_closed) : worms_list() {
    uint8_t worms_amount = 0;
    skt.recvall(&worms_amount, sizeof(worms_amount), was_closed);
    // std::cout << "worms amount:" << std::to_string(worms_amount) <<
    // std::endl;
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
                uint8_t dir, uint8_t state, float angle) {
    Worm worm(player_id, Position(pos_x, pos_y), dir, state, angle);
    worms_list.insert(std::pair<uint8_t, Worm>(worm.get_player_id(), worm));
  }
};

#endif

/*
// Crea un gusano con su posicion actual
struct Worm {
private:
  Position pos;
  uint8_t direction;
  // uint8_t state; // Si está corriendo, disparando, saltando, etc
  // uint16_t id;
  // uint16_t player_id;

public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  Worm() : pos(0, 0), direction(RIGHT) {}

  explicit Worm(Position pos, u_int8_t dir) : pos(pos), direction(dir) {}

  // Constructor para deserializar GameState
  explicit Worm(const std::vector<char> &buf) : pos(0, 0), direction(RIGHT) {
    this->pos = Position(buf);
    uint8_t dir;
    memcpy(&dir, &buf[2 * sizeof(float)], sizeof(uint8_t));
    this->direction = dir;
  }

  float get_pos_x() { return pos.get_position_x(); }

  float get_pos_y() { return pos.get_position_y(); }

  uint8_t get_direction() { return direction; }
};

// Una estructura del estado del juego. Contiene una lista con todos los gusanos
// que se encuentran en la partida
struct GameState {
private:
  std::list<Worm> worms_list;
  // uint16_t players_amount;

public:
  GameState() : worms_list(std::list<Worm>(0)) {}

  explicit GameState(const std::list<Worm> &list) : worms_list(list) {}

  // Constructor que funciona como una deserializacion, recibe la tira de bytes
  // y devuelve un game state
  GameState(Socket &skt, bool *was_closed, std::vector<char> buf)
      : worms_list(0) {
    uint8_t worms_amount = 0;
    skt.recvall(&worms_amount, sizeof(worms_amount), was_closed);

    std::list<Worm> list(0);
    for (int i = 0; i < worms_amount; i++) {
      std::vector<char> buf_worm((2 * sizeof(float)) + 1);
      memcpy(buf_worm.data(), &buf[i * sizeof(buf_worm) + 1], sizeof(buf_worm));
      Worm worm(buf_worm);
      list.push_back(worm);
    }
    this->worms_list = list;
  }

  void serialize(Socket &skt, bool *was_closed) {
    uint8_t worms_amount = worms_list.size();
    std::vector<char> buf(1 + (worms_amount * (2 * sizeof(float) + 1)));
    memcpy(buf.data(), &worms_amount, sizeof(worms_amount));

    int i = 0;
    for (std::list<Worm>::iterator it = worms_list.begin();
         it != worms_list.end(); ++it) {
      // esto habria que hacerlo de otra manera para comprimirlo, sino va a
      // quedar gigante.
      auto pos_x = it->get_pos_x();
      auto pos_y = it->get_pos_y();
      auto dir = it->get_direction();
      memcpy(&buf[(i * 2 * sizeof(float)) + 1], &pos_x, sizeof(float));
      memcpy(&buf[((i * 2 * sizeof(float)) + 2) + 1], &pos_y, sizeof(float));
      memcpy(&buf[((i * 2 * sizeof(float)) + 4) + 1], &dir, sizeof(uint8_t));
      i++;
    }
    skt.sendall(buf.data(), buf.size(), was_closed);
  }

  std::list<Worm> get_worms() { return worms_list; }

  void add_worm(const float &pos_x, const float &pos_y, uint8_t dir) {
    Worm worm(Position(pos_x, pos_y), dir);
    worms_list.push_back(worm);
  }
};
*/