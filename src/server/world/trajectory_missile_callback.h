#ifndef TRAJECTORY_MISSILE_CALLBACK_H
#define TRAJECTORY_MISSILE_CALLBACK_H

#include "../bodies/body.h"

class MissileCallback : public b2RayCastCallback {
 private:
  bool hit_a_wall = false;
  b2Vec2 hit_point = b2Vec2(0, 0);
  float min_fraction = 20;

 public:
  explicit MissileCallback();

  float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

  bool did_hit_wall();

  b2Vec2 get_hit_point();
};

#endif