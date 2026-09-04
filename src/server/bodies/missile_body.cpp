#include "missile_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

MissileBody::MissileBody(const BodyBasicData& basic_data, uint8_t dir,
                         const BodyAdvData& adv_data, b2World* world) :
    Explodable(basic_data, dir, adv_data, MISSILE, world) {}

void MissileBody::update() {
  if (check_boundaries()) {
    explode();
  }
}

bool MissileBody::has_exceeded_width_limit() { return get_pos_x() < 0; }

bool MissileBody::has_exceeded_height_limit() { return get_pos_y() < 0; }

void MissileBody::start_contact_with_beam(b2Fixture self_fixt) { explode(); }
void MissileBody::start_contact_with_worm(b2Fixture self_fixt) { explode(); }
void MissileBody::start_contact_with_missile(b2Fixture self_fixt) { explode(); }
void MissileBody::start_contact_with_grenade(b2Fixture self_fixt) { explode(); }
void MissileBody::start_contact_with_water(b2Fixture self_fixt) { explode(); }

BODY_TYPES MissileBody::get_type() { return MISSILE; }

MissileBody::~MissileBody() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
