#ifndef WATER_BODY_H
#define WATER_BODY_H

#include "body.h"
#include "../../common/game_constants.h"

class WaterBody : public Body {
 public:
  explicit WaterBody(const BodyBasicData& basic_data, const BodyAdvData& adv_data,
                     b2World* world);

  float explosion_intersect_value(float fraction) override;

  void update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal,
                                    float fraction) override;
  BodyExplosionInfo get_explosion_info() override;

  BODY_TYPES get_type() override;

  WaterBody(const WaterBody&) = delete;
  WaterBody& operator=(const WaterBody&) = delete;
};

#endif
