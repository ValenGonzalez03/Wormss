#ifndef MISSILE_BODY_H
#define MISSILE_BODY_H

#include "../../common/game_constants.h"
#include "explodable.h"
#include "box2d/box2d.h"
#include <iostream>
#include <stdio.h>

class MissileBody : public Explodable {
 private:
  bool m_contacting = false;

 public:
  explicit MissileBody(const BodyBasicData& basic_data, uint8_t dir,
                       const BodyAdvData& adv_data, b2World* world);

  void update() override;

  bool has_exceeded_width_limit();

  bool has_exceeded_height_limit();

  void start_contact_with_worm(b2Fixture self_fixture) override;
  void start_contact_with_beam(b2Fixture self_fixture) override;
  void start_contact_with_missile(b2Fixture self_fixture) override;
  void start_contact_with_grenade(b2Fixture self_fixture) override;
  void start_contact_with_water(b2Fixture self_fixture) override;

  BODY_TYPES get_type() override;

  ~MissileBody();

  MissileBody(const MissileBody&) = delete;
  MissileBody& operator=(const MissileBody&) = delete;
};

#endif
