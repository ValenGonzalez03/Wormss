#include "missile_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

MissileBody::MissileBody(b2World *world, float pos_x, float pos_y, float angle, uint8_t dir, uint8_t id)
    : Explodable(world, pos_x, pos_y, angle, MISSILE_WIDTH, MISSILE_HEIGHT, dir, id, MISSILE) {}

void MissileBody::update() {
  check_boundaries();
}

bool MissileBody::has_exceeded_width_limit() { return get_pos_x() < 0; }

bool MissileBody::has_exceeded_height_limit() { return get_pos_y() < 0; }

void MissileBody::touch_beam(BeamBody* beam) { /* NADA */ }
void MissileBody::touch_worm(WormBody* worm) { /* NADA */ }
void MissileBody::touch_missile(MissileBody* missile) { /* MISIL DEBE EXPLOTAR */ }
void MissileBody::touch_grenade(GrenadeBody* grenade) { /* NADA */ }

void MissileBody::stop_touching_worm(WormBody* worm) { /* NADA */ }
void MissileBody::stop_touching_beam(BeamBody* beam) { /* NADA */ }
void MissileBody::stop_touching_missile(MissileBody* missile) { /* NADA */ }
void MissileBody::stop_touching_grenade(GrenadeBody* grenade) { /* NADA */ }

BODY_TYPES MissileBody::get_type() { return MISSILE; }

MissileBody::~MissileBody() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
