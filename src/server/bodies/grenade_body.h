#ifndef GRENADE_BODY_H
#define GRENADE_BODY_H

#include "../../common/game_constants.h"
#include "explodable.h"
#include "box2d/box2d.h"
#include <iostream>
#include <stdio.h>

#define TIME_DETONATION 5 * 60  // 5 segundos a 60 FPS

class GrenadeBody : public Explodable {
 private:
  bool m_contacting = false;
  bool started_countdown = false;
  int frames_counted = 0;

  void check_grenade_in_movement();

 public:
  explicit GrenadeBody(const BodyBasicData& basic_data, uint8_t dir,
                       const BodyAdvData& adv_data, b2World* world);

  void update() override;

  bool has_exceeded_width_limit();

  bool has_exceeded_height_limit();

  BODY_TYPES get_type() override;

  ~GrenadeBody();

  GrenadeBody(const GrenadeBody&) = delete;
  GrenadeBody& operator=(const GrenadeBody&) = delete;
};

#endif
