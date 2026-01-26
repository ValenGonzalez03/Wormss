#include "explodable.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

Explodable::Explodable(BodyBasicData basic_data, uint8_t dir, BodyAdvData adv_data, BODY_TYPES type, b2World* world) :
    DynamicBody(basic_data, adv_data, type, world), direction(dir) {}

void Explodable::apply_initial_impulse(float initial_force, float aim_angle) {
  float vel_x = cos(aim_angle) * initial_force;
  float vel_y = sin(aim_angle) * initial_force;
  b2Vec2 dir_vec = (direction == RIGHT ? b2Vec2(vel_x, vel_y) : b2Vec2(-vel_x, -vel_y));
  std::cout << "angle: " << aim_angle;
  std::cout << "  Dir: (" << dir_vec.x << ", " << dir_vec.y << ")" << std::endl;
  body->ApplyLinearImpulse(dir_vec, body->GetWorldCenter(), true);
}

float Explodable::explosion_intersect_value(float fraction) { return fraction; }

void Explodable::update_explosion_ray_contact(b2Vec2& point, b2Vec2& normal, float fraction) { /* NADA */ }

BodyExplosionInfo Explodable::get_explosion_info() { return BodyExplosionInfo{b2Vec2(0, 0), b2Vec2(0, 0), 0}; }

uint8_t Explodable::get_direction() { return direction; }

uint8_t Explodable::get_id() { return id; }

void Explodable::explode() { exploded = true; }

bool Explodable::has_exploded() { return exploded; }

Explodable::~Explodable() { free(reinterpret_cast<UserData*>(body->GetUserData().pointer)); }
