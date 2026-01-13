#include "static_body.h"
#include "box2d/box2d.h"

StaticBody::StaticBody(b2World* world, const float pos_x, const float pos_y, float angle, const float width, const float height, float density, float friction, BODY_TYPES type)
  : Body(world, pos_x, pos_y, angle, width, BEAM_HEIGHT, 1.0f, 0.5f, b2_staticBody, type) {
    affected_by_explosions = false;
  };

bool StaticBody::is_affected_by_explosions() {
  return affected_by_explosions;
}

void StaticBody::apply_impulse(const b2Vec2 &impulse, const b2Vec2 &point) {
  body->ApplyLinearImpulse(impulse, point, true);
}
