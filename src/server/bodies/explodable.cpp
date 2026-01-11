#include "explodable.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

Explodable::Explodable(b2World *world, float pos_x, float pos_y, float angle, float width, float height, 
                        uint8_t dir, uint8_t id, BODY_TYPES type)
    : Body(world, pos_x, pos_y, angle, width, height, 1.0f, 0.3f), direction(dir), id(id) {
  b2BodyDef bodyDef;
  bodyDef.bullet = true;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos_x, pos_y); // Ajusto la distancia inicial un poco mas adelante para que no choque con el gusano.
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

}

void Explodable::apply_initial_impulse(float initial_force, float aim_angle) {
  float vel_x = cos(aim_angle) * initial_force;
  float vel_y = sin(aim_angle) * initial_force;
  b2Vec2 dir_vec = (direction == RIGHT ? b2Vec2(vel_x, vel_y) : b2Vec2(-vel_x, -vel_y)) ;
  std::cout << "angle: " << aim_angle;
  std::cout << "  Dir: (" << dir_vec.x << ", " << dir_vec.y << ")" << std::endl;
  body->ApplyLinearImpulse(dir_vec, body->GetWorldCenter(), true);
}

float Explodable::explosion_intersect_value(float fraction) {
  return fraction;
}

void Explodable::update_explosion_ray_contact(b2Vec2& point, b2Vec2& normal, float fraction) {
  /* NADA */
}

BodyExplosionInfo Explodable::get_explosion_info() {
  return BodyExplosionInfo {b2Vec2(0,0), b2Vec2(0,0), 0};
}

b2Vec2 Explodable::get_position() { return body->GetPosition(); }

float Explodable::get_pos_x() { return body->GetPosition().x; }

float Explodable::get_pos_y() { return body->GetPosition().y; }

float Explodable::get_angle() { return body->GetAngle(); }

uint8_t Explodable::get_direction() { return direction; }

uint8_t Explodable::get_id() { return id; }

void Explodable::explode() {
  exploded = true;
}

bool Explodable::has_exploded() {
  return exploded;
}

Explodable::~Explodable() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
