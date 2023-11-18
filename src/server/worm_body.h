#ifndef WORM_BODY_H
#define WORM_BODY_H

#include "box2d/box2d.h"
#include <stdio.h>

#define LEFT 0
#define RIGHT 1

class WormBody {
private:
  b2World* world;
  b2Body* body;
  float pos_x; 
  float pos_y;
  uint8_t direction = RIGHT;
  float angle = 0;
  float vel = 0.2;
  const float width = 1;
  const float height = 1;
  const float density = 1;
  const float friction = 1;

public:
  explicit WormBody(b2World* world, float pos_x, float pos_y);
  
  void move_left();
  
  void move_right();
  
  b2Vec2 get_position();
  
  float get_pos_x();
  
  float get_pos_y();
  
  uint8_t get_direction();
  
  WormBody(const WormBody&) = delete;
  WormBody& operator=(const WormBody&) = delete;
};

#endif
