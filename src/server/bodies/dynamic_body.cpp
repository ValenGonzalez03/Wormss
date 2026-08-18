#include "dynamic_body.h"
#include "box2d/box2d.h"

DynamicBody::DynamicBody(const BodyBasicData &basic_data, const BodyAdvData &adv_data,
                         BODY_TYPES type, b2World *world) :
    Body(basic_data, adv_data, type, b2_dynamicBody, world) {
  affected_by_explosions = true;
}

void DynamicBody::take_damage(int amount) {}
