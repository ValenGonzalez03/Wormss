#ifndef EXPLODABLE_H
#define EXPLODABLE_H

#include "../../common/game_constants.h"
#include "dynamic_body.h"
#include "box2d/box2d.h"
#include <iostream>
#include <stdio.h>

class Explodable : public DynamicBody {
 private:
  uint8_t direction;
  float charge_intensity = 0.0f;
  bool m_contacting = false;
  bool exploded = false;

 public:
  explicit Explodable(const BodyBasicData& basic_data, uint8_t direction,
                      const BodyAdvData& adv_data, BODY_TYPES type, b2World* world);

  void apply_initial_impulse(float intial_force, float shooting_angle);

  float explosion_intersect_value(float fraction) override;

  void update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& normal,
                                    float fraction) override;

  BodyExplosionInfo get_explosion_info() override;

  uint8_t get_direction();

  float get_charge_intensity() const;

  virtual void update() = 0;

  void touch_worm(WormBody* worm) override = 0;
  void touch_beam(BeamBody* beam) override = 0;
  void touch_missile(MissileBody* missile) override = 0;
  void touch_grenade(GrenadeBody* grenade) override = 0;

  void stop_touching_worm(WormBody* worm) override = 0;
  void stop_touching_beam(BeamBody* beam) override = 0;
  void stop_touching_missile(MissileBody* missile) override = 0;
  void stop_touching_grenade(GrenadeBody* grenade) override = 0;

  void explode();

  bool has_exploded();

  BODY_TYPES get_type() override = 0;

  ~Explodable();

  Explodable(const Explodable&) = delete;
  Explodable& operator=(const Explodable&) = delete;
};

#endif
