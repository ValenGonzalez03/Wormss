#ifndef BODY_H
#define BODY_H

#include "box2d/box2d.h"

class WormBody;
class WaterBody;
class Weapon;

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

  virtual void start_contact_with(Body* another_body) = 0;
  //virtual void start_contact_with(WormBody* worm) = 0;
  //virtual void start_contact_with(WaterBody* worm) = 0;
  //virtual void start_contact_with(BeamBody* beam) = 0;
  //virtual void start_contact_with(Weapon* weamon) = 0;
  virtual void end_contact_with(Body* another_body) = 0;
  virtual int get_type() = 0;
	
  Body(const Body &) = delete;
  Body &operator=(const Body &) = delete;
};

#endif
