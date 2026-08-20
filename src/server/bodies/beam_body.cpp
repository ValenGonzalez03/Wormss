#include "box2d/box2d.h"
#include "beam_body.h"
#include "worm_body.h"
#include "missile_body.h"
#include <stdio.h>

BeamBody::BeamBody(const BodyBasicData& basic_data, const BodyAdvData& adv_data,
                   b2World* world) :
    Body(basic_data, adv_data, BEAM, b2_staticBody, world) {
  affected_by_explosions = false;
}

float BeamBody::explosion_intersect_value(float fraction) { return fraction; }

void BeamBody::update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal,
                                            float fraction) { /* NADA */ }

BodyExplosionInfo BeamBody::get_explosion_info() {
  return BodyExplosionInfo{b2Vec2(0, 0), b2Vec2(0, 0), 0};
}

void BeamBody::print_beam() {
  std::cout << "BEAM =>  ";
  std::cout << "pos_x: " << get_pos_x();
  std::cout << ",  pos_y: " << get_pos_y();
  std::cout << ",  width: " << width;
  std::cout << ",  height: " << height;
  std::cout << ",  angle: " << angle << std::endl;
}

BODY_TYPES BeamBody::get_type() { return BEAM; }

BeamAttr BeamBody::get_attr() {
  return BeamAttr{get_pos_x(), get_pos_y(), get_angle(), width};
}
