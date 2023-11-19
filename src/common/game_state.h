#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <cstdint>
#include <list>
#include <vector>

#include "position.h"
#include "socket.h"

#define LEFT 0
#define RIGHT 1

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

#endif

/*
struct Worm {
private:
  Position pos;
  uint8_t direction;
  uint8_t state; // Si está corriendo, disparando, saltando, etc
  // uint16_t id;
  // uint16_t player_id;

public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  Worm() : pos(0, 0), direction(RIGHT), state(0) {}

  explicit Worm(Position pos, u_int8_t dir, uint8_t st) : pos(pos), direction(dir), state(st) {}

  Worm(Socket &skt) : pos(0,0) {
    bool was_closed = false;
    deserialize(skt, &was_closed);
  }

  void deserialize(Socket &skt, bool* was_closed) {
    //recibe la pos, la direccion, el state y devuelve un worm
    float pos_x;
    float pos_y;
    //recvall() y guardo en las pos_x y pos_y
    Position position(pos_x, pos_y);
    this->pos = position;
    skt.recvall(&(this->direction), sizeof(this->direction), was_closed);
    skt.recvall(&(this->state), sizeof(this->state), was_closed);
  }

  void serialize(Socket &skt, bool* was_closed) {
    //Hago send de la position
    skt.sendall(&(this->direction), sizeof(this->direction), was_closed);
    skt.sendall(&(this->state), sizeof(this->state), was_closed);
  }
};


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
    for (int i=0; i<worms_amount; i++) {
      Worm worm(skt);
      worms_list.push_back(worm);
    }
  }

};
*/