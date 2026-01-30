#ifndef STATIC_BODY_H
#define STATIC_BODY_H

#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include "body.h"

class StaticBody : public Body {
 public:
  StaticBody(const BodyBasicData& basic_data, const BodyAdvData& adv_data, BODY_TYPES type, b2World* world);

  void touch_worm(WormBody* worm) override = 0;
  void touch_beam(BeamBody* beam) override = 0;
  void touch_missile(MissileBody* missile) override = 0;
  void touch_grenade(GrenadeBody* grenade) override = 0;

  void stop_touching_worm(WormBody* worm) override = 0;
  void stop_touching_beam(BeamBody* beam) override = 0;
  void stop_touching_missile(MissileBody* missile) override = 0;
  void stop_touching_grenade(GrenadeBody* grenade) override = 0;

  float explosion_intersect_value(float fraction) override = 0;

  void update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal, float fraction) override = 0;

  BodyExplosionInfo get_explosion_info() override = 0;

  BODY_TYPES get_type() override = 0;

  StaticBody(const StaticBody&) = delete;
  StaticBody& operator=(const StaticBody&) = delete;
};

#endif
