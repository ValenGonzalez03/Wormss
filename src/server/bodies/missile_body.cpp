#include "missile_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

MissileBody::MissileBody(const BodyBasicData& basic_data, uint8_t dir, const BodyAdvData& adv_data,
                         b2World* world) : Explodable(basic_data, dir, adv_data, MISSILE, world) {}

void MissileBody::update() { check_boundaries(); }

bool MissileBody::has_exceeded_width_limit() { return get_pos_x() < 0; }

bool MissileBody::has_exceeded_height_limit() { return get_pos_y() < 0; }

void MissileBody::touch_beam(BeamBody* beam) { explode(); }
void MissileBody::touch_worm(WormBody* worm) { explode(); }
void MissileBody::touch_missile(MissileBody* missile) { explode(); }
void MissileBody::touch_grenade(GrenadeBody* grenade) { /* NADA */ }

void MissileBody::stop_touching_worm(WormBody* worm) { /* NADA */ }
void MissileBody::stop_touching_beam(BeamBody* beam) { /* NADA */ }
void MissileBody::stop_touching_missile(MissileBody* missile) { /* NADA */ }
void MissileBody::stop_touching_grenade(GrenadeBody* grenade) { /* NADA */ }

BODY_TYPES MissileBody::get_type() { return MISSILE; }

MissileBody::~MissileBody() { free(reinterpret_cast<UserData*>(body->GetUserData().pointer)); }
