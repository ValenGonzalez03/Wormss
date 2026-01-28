#include "box2d/box2d.h"
#include "beam_body.h"
#include "worm_body.h"
#include "missile_body.h"
#include <stdio.h>

BeamBody::BeamBody(BodyBasicData basic_data, BodyAdvData adv_data, b2World* world) :
    StaticBody(basic_data, adv_data, BEAM, world) {
  /*polygonShape.SetAsBox(width, width, b2Vec2(0, 0), angleInRadians);
    fixtureDef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixtureDef);
    b2FixtureUserData data = footSensorFixture->GetUserData();
    data.pointer = (uintptr_t)3;*/
}

// void BeamBody::start_contact_with(Body* another_body) {
//  if (another_body->get_type() == WORM) {}
// }

float BeamBody::explosion_intersect_value(float fraction) { return fraction; }

void BeamBody::update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal, float fraction) { /* NADA */ }

BodyExplosionInfo BeamBody::get_explosion_info() { return BodyExplosionInfo{b2Vec2(0, 0), b2Vec2(0, 0), 0}; }

void BeamBody::touch_beam(BeamBody* beam) { /* NADA */ }
void BeamBody::touch_worm(WormBody* worm) { /* NADA */ }
void BeamBody::touch_missile(MissileBody* missile) { /* NADA */ }
void BeamBody::touch_grenade(GrenadeBody* grenade) { /* NADA */ }

void BeamBody::stop_touching_worm(WormBody* worm) { /* NADA */ }
void BeamBody::stop_touching_beam(BeamBody* beam) { /* NADA */ }
void BeamBody::stop_touching_missile(MissileBody* missile) { /* NADA */ }
void BeamBody::stop_touching_grenade(GrenadeBody* grenade) { /* NADA */ }

void BeamBody::print_beam() {
  std::cout << "BEAM =>  ";
  std::cout << "pos_x: " << get_pos_x();
  std::cout << ",  pos_y: " << get_pos_y();
  std::cout << ",  width: " << width;
  std::cout << ",  height: " << height;
  std::cout << ",  angle: " << angle << std::endl;
}

BODY_TYPES BeamBody::get_type() { return BEAM; }

BeamAttr BeamBody::get_attr() { return BeamAttr{get_pos_x(), get_pos_y(), get_angle(), width}; }
