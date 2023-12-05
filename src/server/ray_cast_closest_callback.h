#ifndef RAY_CAST_CLOSEST_CALLBACK_H
#define RAY_CAST_CLOSEST_CALLBACK_H

#include "box2d/box2d.h"

class RayCastClosestCallback  : public b2RayCastCallback {
private:
	bool hit;
	b2Vec2 point;
	b2Vec2 normal;

public:
	RayCastClosestCallback();

	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction);
};

#endif
