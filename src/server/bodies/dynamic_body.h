#ifndef DYNAMIC_BODY_H
#define DYNAMIC_BODY_H

#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include "body.h"

class DynamicBody : public Body {
public:
  DynamicBody(b2World* world, const float pos_x, const float pos_y, float angle, const float width, const float height, float density, float friction, BODY_TYPES type);

  virtual void touch_worm(WormBody* worm) = 0;
  virtual void touch_beam(BeamBody* beam) = 0;
  virtual void touch_missile(MissileBody* missile) = 0;
  virtual void touch_grenade(GrenadeBody* grenade) = 0;

  virtual void stop_touching_worm(WormBody* worm) = 0;
  virtual void stop_touching_beam(BeamBody* beam) = 0;
  virtual void stop_touching_missile(MissileBody* missile) = 0;
  virtual void stop_touching_grenade(GrenadeBody* grenade) = 0;

  void apply_impulse(const b2Vec2 &impulse, const b2Vec2 &point);

  virtual float explosion_intersect_value(float fraction) = 0;

  virtual void update_explosion_ray_contact(b2Vec2& point, b2Vec2& normal, float fraction) = 0;

  virtual BodyExplosionInfo get_explosion_info() = 0;

  virtual BODY_TYPES get_type() = 0;

  bool is_affected_by_explosions();
	
  DynamicBody(const DynamicBody &) = delete;
  DynamicBody &operator=(const DynamicBody &) = delete;
};

#endif
