#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include <list>
#include <vector>
#include <cstdint>

#include "socket.h"
#include "../client/client_position.h"

// Crea un gusano con su posicion actual
struct Worm {
private:
  Position pos;

public:
  // Default constructor (PARA QUE COMPILE, REVISAR!!!!)
  Worm() : pos(0, 0) {}

  // Constructor para deserializar GameState
  explicit Worm(std::vector<char> buf) : pos(0, 0) {
    this->pos = Position(buf);
  }

  char *data() { return pos.get_data(); }
};

// Una estructura del estado del juego. Contiene una lista con todos los gusanos
// que se encuentran en la partida
struct GameState {
private:
  std::list<Worm> worms_list;

public:
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
    std::vector<char> buf(worms_amount * sizeof(Worm) + 1);
    memcpy(buf.data(), &worms_amount, sizeof(worms_amount));

    int i = 0;
    for (std::list<Worm>::iterator it = worms_list.begin();
         it != worms_list.end(); ++it) {
      memcpy(&buf[i * sizeof(Worm) + 1], it->data(), sizeof(Worm));
      i++;
    }
    skt.sendall(buf.data(), buf.size(), was_closed);
  }
};


#endif
