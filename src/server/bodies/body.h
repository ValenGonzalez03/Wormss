#ifndef BODY_H
#define BODY_H

#include "box2d/box2d.h"

class WormBody;
class BeamBody;
class MissileBody;


enum BODY_TYPES {
    WORM = 0,
    WATER = 1,
    BEAM = 2,
    MISSILE = 3
};

struct UserData {
  BODY_TYPES type;
  void* pointer;
};

class Body {
protected:
  b2World* world;
	b2Body* body;
	const float pos_x; 
	const float pos_y;
	float angle;
	const float width;
	const float height;
	float density;
	float friction;

public:
  Body(b2World* world, const float pos_x, const float pos_y, int angle, const float width, const float height, float density, float friction);

  void start_contact_with(Body* another_body);
  void touch(Body *other);
  virtual void touch_worm(WormBody* worm) = 0;
  virtual void touch_beam(BeamBody* beam) = 0;
  virtual void touch_missile(MissileBody* missile) = 0;

  void end_contact_with(Body* another_body);
  void stop_touching(Body *other);
  virtual void stop_touching_worm(WormBody* worm) = 0;
  virtual void stop_touching_beam(BeamBody* beam) = 0;
  virtual void stop_touching_missile(MissileBody* missile) = 0;

  virtual float explosion_intersect_value(float fraction) = 0;

  virtual void apply_explosion(b2Vec2& point, b2Vec2& normal) = 0;

  b2Body* get_body();
  virtual int get_type() = 0;
  //virtual void start_contact_with(Weapon* weamon) = 0;
	
  Body(const Body &) = delete;
  Body &operator=(const Body &) = delete;
};

#endif
