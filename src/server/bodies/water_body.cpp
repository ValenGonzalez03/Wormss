#include "water_body.h"

WaterBody::WaterBody(const BodyBasicData& basic_data, const BodyAdvData& adv_data,
                     b2World* world) :
    Body(basic_data, adv_data, WATER, b2_staticBody, world) {}


float WaterBody::explosion_intersect_value(float fraction) { return 1; }

void WaterBody::update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal,
                                             float fraction) { /* NADA */ }

BodyExplosionInfo WaterBody::get_explosion_info() {
  return BodyExplosionInfo{b2Vec2(0, 0), b2Vec2(0, 0), 0};
}

BODY_TYPES WaterBody::get_type() { return WATER; }
