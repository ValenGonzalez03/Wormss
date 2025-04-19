#ifndef CLIENT_POSITION_H_
#define CLIENT_POSITION_H_

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

// Crea una posicion en metros para un objeto
struct Position {
private:
  float _pos_x;
  float _pos_y;

public:
  explicit Position(float position_x, float position_y)
      : _pos_x(position_x), _pos_y(position_y) {}

  /*
    // Constructor para deserializar GameState
    explicit Position(const std::vector<char> &buf) : _pos_x(0), _pos_y(0) {
      float pos_x = 0;
      float pos_y = 0;
      memcpy(&pos_x, buf.data(), sizeof(float));
      memcpy(&pos_y, &buf[sizeof(float)], sizeof(float));
      this->_pos_x = pos_x;
      this->_pos_y = pos_y;
    }
  */

  // Devuelve la pos_x en metros
  float get_position_x() { return this->_pos_x; }

  // Devuelve la pos_y en metros
  float get_position_y() { return this->_pos_y; }

  // Convierte la pos de m a px y la devuelve
  // int get_position_x_in_px() {}

  // int get_position_y_in_px() {}

  //~Position() { free(data); }
};

static const float PIXEL_PER_METER = 23.33f; // 23,3 periodico
static float WORLD_HEIGHT = 360.0f;

static int convert_meters_to_pixels_x(float pos_x) {
  return pos_x * PIXEL_PER_METER;
}

static int convert_meters_to_pixels_y(float pos_y) {
  return WORLD_HEIGHT - pos_y * PIXEL_PER_METER;
}

static float convert_pixels_to_meters_x(int pos_x) {
  return pos_x / PIXEL_PER_METER;
}

static float convert_pixels_to_meters_y(int pos_y) {
  return (WORLD_HEIGHT - pos_y) / PIXEL_PER_METER;
}

#endif
