#ifndef EXPLOSION_CALLBACK_H
#define EXPLOSION_CALLBACK_H

#include "box2d/box2d.h"
#include "../bodies/body.h"
#include "server_explosion.h"
#include <list>



class ExplosionCallback : public b2RayCastCallback {
 private:
  int ray_number;
  Explosion& explosion;
  float smallest_intersection = 1;
  std::list<BodyRayInfo> bodies;

 public:
  ExplosionCallback(int ray_number, Explosion& explosion);

  float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

  void evaluate_contact_for_bodies();

  float get_ray_fraction();
};

#endif
