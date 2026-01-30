#include "static_body.h"
#include "box2d/box2d.h"

StaticBody::StaticBody(const BodyBasicData &basic_data, const BodyAdvData &adv_data, BODY_TYPES type, b2World *world) :
    Body(basic_data, adv_data, type, b2_staticBody, world) {
  affected_by_explosions = false;
}
