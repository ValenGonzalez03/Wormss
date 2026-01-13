#include "body.h"
#include "box2d/box2d.h"
#include <iostream>

Body::Body(b2World* world, const float pos_x, const float pos_y, const float angle, const float width, const float height, float density, float friction, b2BodyType body_type, BODY_TYPES type)
  : width(width), height(height), angle(angle) {
  b2BodyDef bodyDef;
  bodyDef.type = body_type;
  bodyDef.position.Set(pos_x, pos_y);
  //std::cout << "Creating body of type " << type << " with angle " << angle << std::endl;
  bodyDef.angle = angle;
  body = world->CreateBody(&bodyDef);


  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width / 2, height / 2);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;

  body->CreateFixture(&fixtureDef);

  UserData* data = new UserData {type, this};
  body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
};

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
  case GRENADE_BODY: {
    GrenadeBody* grenade = reinterpret_cast<GrenadeBody*>(other);
    touch_grenade(grenade);
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
  case GRENADE_BODY: {
    GrenadeBody* grenade = reinterpret_cast<GrenadeBody*>(other);
    stop_touching_grenade(grenade);
    break;
  }
  
  default:
    break;
  }
}

void Body::check_boundaries() {
  if ((get_pos_x() < 0) || (get_pos_x() > 25)) { // Excedio limite horizontal
    body->SetTransform(b2Vec2(5, 25), 0); // Por ahora solo fuerzo a que reaparezca mas arriba y a la derecha
  }
  if ((get_pos_y() < 0) || (get_pos_y() > WORLD_HEIGHT)) { // Excedio limite vertical
    body->SetTransform(b2Vec2(get_pos_x(), 25), 0); // Por ahora solo fuerzo a que reaparezca 25 metros mas arriba
  }
}

bool Body::is_affected_by_explosions() {
  return affected_by_explosions;
}

void Body::apply_impulse(const b2Vec2 &impulse, const b2Vec2 &point) {
  body->ApplyLinearImpulse(impulse, point, true);
}

float Body::get_pos_x() {
  return body->GetPosition().x;
}

float Body::get_pos_y() {
  return body->GetPosition().y;
}

float Body::get_width() {
  return width;
}

float Body::get_height() {
  return height;
}

float Body::get_angle() {
  return body->GetAngle();
}

UserData* Body::get_user_data() {
  return reinterpret_cast<UserData*>(body->GetUserData().pointer);
}
