#ifndef MISSILE_BODY_H
#define MISSILE_BODY_H

#include "../../common/game_constants.h"
#include "body.h"
#include "box2d/box2d.h"
#include <iostream>
#include <stdio.h>

class MissileBody : public Body {
private:
  int id;
  float initial_force;
  bool m_contacting = false;

public:

  explicit MissileBody(b2World* world, float pos_x, float pos_y, float initial_force, float angle, int id);

  void apply_initial_impulse(float shooting_angle);

  b2Vec2 get_position();

  float get_pos_x();

  float get_pos_y();

  float get_angle();

  int get_id();

  void update();

  bool has_exceeded_width_limit();

  bool has_exceeded_height_limit();

  void start_contact_with(Body *another_body) override;

  void end_contact_with(Body *another_body) override;

  void hit_a_surface();

  void move_away_from_surface();
  
  int get_type() override;

  ~MissileBody();
  
  MissileBody(const MissileBody&) = delete;
  MissileBody& operator=(const MissileBody&) = delete;
};

#endif
