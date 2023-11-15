#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <cstdint>
#include <list>
#include <vector>

#include "position.h"
#include "socket.h"

// Crea un gusano con su posicion actual
struct Worm {
private:
  Position pos;
  // uint16_t id;
  // uint16_t player_id;
  // uint8_t direction;

public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  Worm() : pos(0, 0) {}

  explicit Worm(Position pos) : pos(pos) {}

  // Constructor para deserializar GameState
  explicit Worm(const std::vector<char> &buf) : pos(0, 0) {
    this->pos = Position(buf);
  }

  float get_pos_x() { return pos.get_position_x(); }

  float get_pos_y() { return pos.get_position_y(); }
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
      std::vector<char> buf_worm(sizeof(Worm));
      memcpy(buf_worm.data(), &buf[i * sizeof(Worm) + 1], sizeof(Worm));
      Worm worm(buf_worm);
      list.push_back(worm);
    }
    this->worms_list = list;
  }

  void serialize(Socket &skt, bool *was_closed) {
    uint8_t worms_amount = worms_list.size();
    std::vector<char> buf((worms_amount * 2 * sizeof(float)) + 1);
    memcpy(buf.data(), &worms_amount, sizeof(worms_amount));

    int i = 0;
    for (std::list<Worm>::iterator it = worms_list.begin();
         it != worms_list.end(); ++it) {
      auto pos_x = it->get_pos_x();
      auto pos_y = it->get_pos_y();
      memcpy(&buf[(i * 2 * sizeof(float)) + 1], &pos_x, sizeof(float));
      memcpy(&buf[((i * 2 * sizeof(float)) + 2) + 1], &pos_y, sizeof(float));
      i++;
    }
    skt.sendall(buf.data(), buf.size(), was_closed);
  }

  void add_worm(const float &pos_x, const float &pos_y) {
    Worm worm(Position(pos_x, pos_y));
    worms_list.push_back(worm);
  }
};

#endif
