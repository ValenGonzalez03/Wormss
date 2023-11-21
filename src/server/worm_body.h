#ifndef WORM_BODY_H
#define WORM_BODY_H

#include "box2d/box2d.h"
#include <stdio.h>
#include "../common/worm_states.h"

#define LEFT 0
#define RIGHT 1

//#define STOPPED 0
//#define MOVING 1

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
  float vel = 0.5;
  const float width = 1;
  const float height = 1;
  const float density = 1;
  const float friction = 0.1;
  
  bool is_moving = false;
  uint8_t state = WORM_STATES::STOPPED;

public:
  explicit WormBody(b2World* world, float pos_x, float pos_y);
  
  void move_left();
  
  void move_right();
  
  void start_moving(const uint8_t &dir);
  
  void stop_moving(); 
  
  b2Vec2 get_position();
  
  float get_pos_x();
  
  float get_pos_y();
  
  uint8_t get_direction();
  
  uint8_t get_state();
  
  void update();
  
  WormBody(const WormBody&) = delete;
  WormBody& operator=(const WormBody&) = delete;
};

#endif
