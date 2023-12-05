#ifndef WORM_BODY_H
#define WORM_BODY_H

#include "../../common/worm_states.h"
#include "body.h"
#include "box2d/box2d.h"
#include <iostream>
#include <stdio.h>

//#define STOPPED 0
//#define MOVING 1

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

class WormBody : public Body {
private:
  uint8_t direction = RIGHT;
  uint8_t aim_direction;
  float aiming_angle = 0;
  float vel = 0.5;
  float jump_vel = 5;
  uint8_t id;

  uint8_t state = WORM_STATES::STOPPED;

  bool m_contacting = false;

public:
  explicit WormBody(b2World *world, float pos_x, float pos_y, uint8_t id);

  void move_left();

  void move_right();

  void apply_horizontal_impulse(float desired_vel);

  void apply_vertical_impulse(float jump_speed);

  void start_moving(const uint8_t &dir);

  void stop_moving();

  void jump_left();

  void jump_right();

  void jump(const uint8_t &dir);

  void start_aiming(const uint8_t &dir);

  void aim_up();

  void aim_down();

  void stop_aiming();

  uint8_t get_id();

  b2Vec2 get_position();

  float get_pos_x();

  float get_pos_y();

  uint8_t get_direction();

  uint8_t get_state();

  float get_aiming_angle();

  void update();

  bool is_facing_left();

  bool is_facing_right();

  bool is_stopped();

  // POR AHORA DE PRUEBA
  void start_contact();
  void end_contact();

  void start_contact_with(Body *another_body) override;

  void start_contact_with(WormBody *another_worm) override;

  void end_contact_with(Body *another_body) override;

  void hit_a_surface();

  void move_away_from_surface();

  WormBody(const WormBody &) = delete;
  WormBody &operator=(const WormBody &) = delete;
};

#endif
