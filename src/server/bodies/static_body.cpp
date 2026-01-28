#include "static_body.h"
#include "box2d/box2d.h"

StaticBody::StaticBody(BodyBasicData basic_data, BodyAdvData adv_data, BODY_TYPES type, b2World *world) :
    Body(basic_data, adv_data, type, b2_staticBody, world) {
  affected_by_explosions = false;
}

bool StaticBody::is_affected_by_explosions() { return affected_by_explosions; }

void StaticBody::apply_impulse(const b2Vec2 &impulse, const b2Vec2 &point) {
  body->ApplyLinearImpulse(impulse, point, true);
}
