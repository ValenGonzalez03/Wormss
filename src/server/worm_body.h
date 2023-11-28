#ifndef WORM_BODY_H
#define WORM_BODY_H

#include "box2d/box2d.h"
#include <stdio.h>
#include "../common/worm_states.h"
#include <iostream>

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
  uint8_t id;
  
  uint8_t state = WORM_STATES::STOPPED;
  
  bool m_contacting = false;

public:
  explicit WormBody(b2World* world, float pos_x, float pos_y, uint8_t id);
  
  void move_left();
  
  void move_right();
  
  void apply_horizontal_impulse(float desired_vel);
  
  void apply_vertical_impulse(float jump_speed);
  
  void start_moving(const uint8_t &dir);
  
  void stop_moving();

  void jump_left();
  
  void jump_right();

  void jump(const uint8_t &dir);

  uint8_t get_id();
  
  b2Vec2 get_position();
  
  float get_pos_x();
  
  float get_pos_y();
  
  uint8_t get_direction();
  
  uint8_t get_state();
  
  void update();
  
  bool is_facing_left();
  
  bool is_facing_right();
  
  bool is_stopped();
  
  // POR AHORA DE PRUEBA
  void start_contact();
  void end_contact();
  
  void start_contact_with(WormBody* another_worm);
  
  void end_contact_with(WormBody* another_worm);
  
  void fall_in_ground();

  void start_jumping();
  
  WormBody(const WormBody&) = delete;
  WormBody& operator=(const WormBody&) = delete;
};

#endif
