#include "grenade_body.h"

#include <stdio.h>

#include "../../common/game_constants.h"
#include "box2d/box2d.h"

GrenadeBody::GrenadeBody(const BodyBasicData& basic_data, uint8_t dir, const BodyAdvData& adv_data, b2World* world) :
    Explodable(basic_data, dir, adv_data, GRENADE_BODY, world) {}

void GrenadeBody::update() {
  check_boundaries();

  if (started_countdown) {
    if (frames_counted >= TIME_DETONATION) {
      explode();
    } else {
      frames_counted++;
    }
  } else {
    check_grenade_in_movement();
  }
}

void GrenadeBody::check_grenade_in_movement() {
  b2Vec2 velocity = body->GetLinearVelocity();
  if (velocity.Length() < 0.01f) {
    started_countdown = true;
  }
}

bool GrenadeBody::has_exceeded_width_limit() { return get_pos_x() < 0; }

bool GrenadeBody::has_exceeded_height_limit() { return get_pos_y() < 0; }

void GrenadeBody::touch_beam(BeamBody* beam) { /* NADA */ }
void GrenadeBody::touch_worm(WormBody* worm) { /* NADA */ }
void GrenadeBody::touch_missile(MissileBody* missile) { /* NADA */ }
void GrenadeBody::touch_grenade(GrenadeBody* grenade) { /* NADA */ }

void GrenadeBody::stop_touching_worm(WormBody* worm) { /* NADA */ }
void GrenadeBody::stop_touching_beam(BeamBody* beam) { /* NADA */ }
void GrenadeBody::stop_touching_missile(MissileBody* missile) { /* NADA */ }
void GrenadeBody::stop_touching_grenade(GrenadeBody* grenade) { /* NADA */ }

BODY_TYPES GrenadeBody::get_type() { return GRENADE_BODY; }

GrenadeBody::~GrenadeBody() { free(reinterpret_cast<UserData*>(body->GetUserData().pointer)); }
