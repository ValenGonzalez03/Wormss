#include "explodable.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

Explodable::Explodable(const BodyBasicData& basic_data, uint8_t dir,
                       const BodyAdvData& adv_data, BODY_TYPES type, b2World* world) :
    Body(basic_data, adv_data, type, b2_dynamicBody, world), direction(dir) {
  affected_by_explosions = true;
}

void Explodable::apply_initial_impulse(float charge_intensity, float aim_angle) {
  this->charge_intensity = charge_intensity;
  float vel_x = cos(aim_angle) * charge_intensity * BLAST_POWER;
  float vel_y = sin(aim_angle) * charge_intensity * BLAST_POWER;
  b2Vec2 dir_vec = (direction == RIGHT ? b2Vec2(vel_x, vel_y) : b2Vec2(-vel_x, -vel_y));
  std::cout << "angle: " << aim_angle;
  std::cout << "  Dir: (" << dir_vec.x << ", " << dir_vec.y << ")" << std::endl;
  body->ApplyLinearImpulse(dir_vec, body->GetWorldCenter(), true);
}

float Explodable::explosion_intersect_value(float fraction) { return fraction; }

void Explodable::update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal,
                                              float fraction) { /* NADA */ }

BodyExplosionInfo Explodable::get_explosion_info() {
  return BodyExplosionInfo{b2Vec2(0, 0), b2Vec2(0, 0), 0};
}

uint8_t Explodable::get_direction() { return direction; }

float Explodable::get_charge_intensity() const { return charge_intensity; }

void Explodable::explode() { exploded = true; }

bool Explodable::has_exploded() { return exploded; }

Explodable::~Explodable() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
