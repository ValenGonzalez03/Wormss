#ifndef BEAM_BODY_H
#define BEAM_BODY_H

#include "box2d/box2d.h"
#include "body.h"
#include "../../common/game_constants.h"
#include <stdio.h>
#include <iostream>

class BeamBody : public Body {
 public:
  // explicit BeamBody(b2World* world, float pos_x, float pos_y);
  // explicit BeamBody(b2World* world, float pos_x, float pos_y, int angle, float length);

  explicit BeamBody(const BodyBasicData& basic_data, const BodyAdvData& adv_data,
                    b2World* world);


  float explosion_intersect_value(float fraction) override;

  void update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal,
                                    float fraction) override;

  BodyExplosionInfo get_explosion_info() override;

  void print_beam();

  BeamAttr get_attr();

  BODY_TYPES get_type() override;

  BeamBody(const BeamBody&) = delete;
  BeamBody& operator=(const BeamBody&) = delete;
};

#endif
