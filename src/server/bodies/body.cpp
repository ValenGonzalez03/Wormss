#include "body.h"
#include "box2d/box2d.h"

Body::Body(b2World* world, const float pos_x, const float pos_y, int angle, const float width, const float height, float density, float friction)
  : world(world), pos_x(pos_x), pos_y(pos_y), angle(angle), width(width), height(height), density(density), friction(friction) {};
