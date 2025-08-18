#ifndef MISSILE_BODY_H
#define MISSILE_BODY_H

#include "../../common/game_constants.h"
#include "body.h"
#include "box2d/box2d.h"
#include <iostream>
#include <stdio.h>

class MissileBody : public Body {
private:
  uint8_t id;
  float initial_force;
  uint8_t missile_direction;
  bool m_contacting = false;
  bool exploded = false;

public:

  explicit MissileBody(b2World* world, float pos_x, float pos_y, float initial_force, float angle, uint8_t missile_direction, 
    uint8_t id);

  void apply_initial_impulse(float shooting_angle);

  b2Vec2 get_position();

  float get_pos_x();

  float get_pos_y();

  float get_angle();

  uint8_t get_direction();

  uint8_t get_id();

  void update();

  bool has_exceeded_width_limit();

  bool has_exceeded_height_limit();

  void touch_worm(WormBody* worm) override;

  void touch_beam(BeamBody* beam) override;

  void touch_missile(MissileBody* missile) override;

  //void start_contact_with(Body *another_body) override;

  //void end_contact_with(Body *another_body) override;

  void stop_touching_worm(WormBody* worm) override;

  void stop_touching_beam(BeamBody* beam) override;
	
  void stop_touching_missile(MissileBody* missile) override;

  void explode();

  bool has_exploded();
  
  int get_type() override;

  ~MissileBody();
  
  MissileBody(const MissileBody&) = delete;
  MissileBody& operator=(const MissileBody&) = delete;
};

#endif
