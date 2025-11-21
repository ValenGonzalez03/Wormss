#include "body.h"
#include "box2d/box2d.h"

Body::Body(b2World* world, const float pos_x, const float pos_y, int angle, const float width, const float height, float density, float friction)
  : world(world), pos_x(pos_x), pos_y(pos_y), angle(angle), width(width), height(height), density(density), friction(friction) {};

void Body::start_contact_with(Body* another_body) {
  another_body->touch(this);
}

void Body::touch(Body *other) {
  auto type = other->get_type();
  switch (type)
  {
  case WORM: {
    WormBody* worm = reinterpret_cast<WormBody*>(other);
    touch_worm(worm);
    break;
  }
  case BEAM: {
    BeamBody* beam = reinterpret_cast<BeamBody*>(other);
    touch_beam(beam);
    break;
  }
  case MISSILE: {
    MissileBody* missile = reinterpret_cast<MissileBody*>(other);
    touch_missile(missile);
    break;
  }
  
  default:
    break;
  }
}

void Body::end_contact_with(Body* another_body) {
  another_body->stop_touching(this);
}

void Body::stop_touching(Body *other) {
  switch (other->get_type())
  {
  case WORM: {
    WormBody* worm = reinterpret_cast<WormBody*>(other);
    stop_touching_worm(worm);
    break;
  }
  case BEAM: {
    BeamBody* beam = reinterpret_cast<BeamBody*>(other);
    stop_touching_beam(beam);
    break;
  }
  case MISSILE: {
    MissileBody* missile = reinterpret_cast<MissileBody*>(other);
    stop_touching_missile(missile);
    break;
  }
  
  default:
    break;
  }
}

bool Body::is_affected_by_explosions() {
  return affected_by_explosions;
}

b2Body* Body::get_body() {
  return body;
}