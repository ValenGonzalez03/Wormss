#ifndef CLIENT_POSITION_H_
#define CLIENT_POSITION_H_

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

// Crea una posicion en pixeles para un objeto
struct Position {
private:
  int16_t _pos_x;
  int16_t _pos_y;

public:
  explicit Position(int16_t position_x, int16_t position_y)
      : _pos_x(position_x), _pos_y(position_y) {}

  // Constructor para deserializar GameState
  explicit Position(const std::vector<char> &buf) : _pos_x(0), _pos_y(0) {
    int16_t pos_x = 0;
    int16_t pos_y = 0;
    memcpy(&pos_x, buf.data(), sizeof(int16_t));
    memcpy(&pos_y, &buf[sizeof(int16_t)], sizeof(int16_t));
    this->_pos_x = pos_x;
    this->_pos_y = pos_y;
  }

  int get_position_x() { return this->_pos_x; }

  int get_position_y() { return this->_pos_y; }

  //~Position() { free(data); }
};

#endif
