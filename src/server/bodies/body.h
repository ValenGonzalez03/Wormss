#ifndef BODY_H
#define BODY_H

#include "box2d/box2d.h"
#include "../../common/game_constants.h"

class WormBody;
class BeamBody;
class MissileBody;
class GrenadeBody;

class World;

struct UserData {
  BODY_TYPES type;
  void* pointer;
};

struct BodyExplosionInfo {
	b2Vec2 apply_point;
	b2Vec2 impulse_dir;
	float fraction_force;
};

class Body {
protected:
	b2Body* body;
  const float width;
  const float height;
  const float angle;
  bool affected_by_explosions = false;

  friend class World;

public:
  Body(b2World* world, const float pos_x, const float pos_y, const float angle, const float width, const float height, float density, float friction, b2BodyType body_type, BODY_TYPES type);

  void start_contact_with(Body* another_body);
  void touch(Body *other);
  virtual void touch_worm(WormBody* worm) = 0;
  virtual void touch_beam(BeamBody* beam) = 0;
  virtual void touch_missile(MissileBody* missile) = 0;
  virtual void touch_grenade(GrenadeBody* grenade) = 0;

  void end_contact_with(Body* another_body);
  void stop_touching(Body *other);
  virtual void stop_touching_worm(WormBody* worm) = 0;
  virtual void stop_touching_beam(BeamBody* beam) = 0;
  virtual void stop_touching_missile(MissileBody* missile) = 0;
  virtual void stop_touching_grenade(GrenadeBody* grenade) = 0;

  void check_boundaries();

  void apply_impulse(const b2Vec2 &impulse, const b2Vec2 &point);

  virtual float explosion_intersect_value(float fraction) = 0;

  virtual void update_explosion_ray_contact(b2Vec2& point, b2Vec2& normal, float fraction) = 0;

  virtual BodyExplosionInfo get_explosion_info() = 0;

  //b2Body* get_body();

  float get_pos_x();

  float get_pos_y();

  float get_width();

  float get_height();

  float get_angle();

  UserData* get_user_data();

  virtual BODY_TYPES get_type() = 0;

  bool is_affected_by_explosions();
  //virtual void start_contact_with(Weapon* weamon) = 0;
	
  Body(const Body &) = delete;
  Body &operator=(const Body &) = delete;
};

#endif
