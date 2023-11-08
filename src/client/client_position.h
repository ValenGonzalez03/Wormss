#ifndef CLIENT_POSITION_H_
#define CLIENT_POSITION_H_

#include <vector>
#include <cstdlib>
#include <cstring>

// Crea una posicion en pixeles para un objeto
struct Position {
private:
  int16_t _pos_x;
  int16_t _pos_y;
  char *data;

  void define_data() {
    std::vector<char> buf(sizeof(Position));
    memcpy(buf.data(), &_pos_x, sizeof(int16_t));
    memcpy(&buf[sizeof(int16_t)], &_pos_y, sizeof(int16_t));
    this->data = buf.data();
  }

public:
  Position(int16_t position_x, int16_t position_y)
      : _pos_x(position_x), _pos_y(position_y), data(nullptr) {
    define_data();
  }

  // Constructor para deserializar GameState
  explicit Position(std::vector<char> buf)
      : _pos_x(0), _pos_y(0), data(nullptr) {
    int16_t pos_x = 0;
    int16_t pos_y = 0;
    memcpy(&pos_x, buf.data(), sizeof(int16_t));
    memcpy(&pos_y, &buf[sizeof(int16_t)], sizeof(int16_t));
    this->_pos_x = pos_x;
    this->_pos_y = pos_y;
    define_data();
  }

  int get_position_x() {
    return this->_pos_x;
  }

  int get_position_y() {
    return this->_pos_y;
  }

  char *get_data() { return this->data; }

  //~Position() { free(data); }
};

#endif
