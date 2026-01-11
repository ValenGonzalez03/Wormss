#ifndef EXPLODABLE_H
#define EXPLODABLE_H

#include "../../common/game_constants.h"
#include "body.h"
#include "box2d/box2d.h"
#include <iostream>
#include <stdio.h>

class Explodable : public Body {
private:
  uint8_t id;
  uint8_t direction;
  bool m_contacting = false;
  bool exploded = false;

public:

  explicit Explodable(b2World* world, float pos_x, float pos_y, float angle, float width, float height,  
                        uint8_t direction, uint8_t id, BODY_TYPES type);

  void apply_initial_impulse(float intial_force, float shooting_angle);

  float explosion_intersect_value(float fraction);

  void update_explosion_ray_contact(b2Vec2& point, b2Vec2& normal, float fraction);

  BodyExplosionInfo get_explosion_info() override;

  b2Vec2 get_position();

  float get_pos_x();

  float get_pos_y();

  float get_angle();

  uint8_t get_direction();

  uint8_t get_id();

  virtual void update() = 0;

  virtual void touch_worm(WormBody* worm) = 0;
  virtual void touch_beam(BeamBody* beam) = 0;
  virtual void touch_missile(MissileBody* missile) = 0;
  virtual void touch_grenade(GrenadeBody* grenade) = 0;

  virtual void stop_touching_worm(WormBody* worm) = 0;
  virtual void stop_touching_beam(BeamBody* beam) = 0;
  virtual void stop_touching_missile(MissileBody* missile) = 0;
  virtual void stop_touching_grenade(GrenadeBody* grenade) = 0;

  void explode();

  bool has_exploded();
  
  virtual BODY_TYPES get_type() = 0;

  ~Explodable();
  
  Explodable(const Explodable&) = delete;
  Explodable& operator=(const Explodable&) = delete;
};

#endif
