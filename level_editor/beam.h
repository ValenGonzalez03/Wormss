#ifndef BEAM_H_
#define BEAM_H_

#include <SDL2pp/SDL2pp.hh>

class Beam{
private:
  float pos_x;
  float pos_y;
  float angle;
  float length;
public:
  Beam(float posx, float posy, float angle, float length) : 
    pos_x(posx), pos_y(posy), angle(angle), length(length) {}
  
  float get_pos_x() {
    return this->pos_x;
  }

  float get_pos_y() {
    return this->pos_y;
  }

  float get_angle() {
    return this->angle;
  }

  float get_length() {
    return this->length;
  }

  void render(SDL2pp::Renderer &renderer) {

  }
};

#endif