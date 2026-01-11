#include "grenade_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

GrenadeBody::GrenadeBody(b2World *world, float pos_x, float pos_y, float angle, uint8_t dir, uint8_t id)
    : Explodable(world, pos_x, pos_y, angle, GRENADE_WIDTH, GRENADE_HEIGHT, dir, id, GRENADE_BODY) {}

void GrenadeBody::update() {
  if (has_exceeded_width_limit()) {
    body->SetTransform(b2Vec2(15, 25), 0); // Por ahora solo fuerzo a que reaparezca mas arriba y a la derecha
  }
  if (has_exceeded_height_limit()) {
    body->SetTransform(b2Vec2(get_pos_x(), 25), 0); // Por ahora solo fuerzo a que reaparezca 25 metros mas arriba
  }

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
void GrenadeBody::touch_missile(MissileBody* missile) { /* MISIL DEBE EXPLOTAR */ }
void GrenadeBody::touch_grenade(GrenadeBody* grenade) { /* NADA */ }

void GrenadeBody::stop_touching_worm(WormBody* worm) { /* NADA */ }
void GrenadeBody::stop_touching_beam(BeamBody* beam) { /* NADA */ }
void GrenadeBody::stop_touching_missile(MissileBody* missile) { /* NADA */ }
void GrenadeBody::stop_touching_grenade(GrenadeBody* grenade) { /* NADA */ }

BODY_TYPES GrenadeBody::get_type() { return GRENADE_BODY; }

GrenadeBody::~GrenadeBody() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
