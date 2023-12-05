#include "ray_cast_closest_callback.h"

RayCastClosestCallback::RayCastClosestCallback(): hit(false) {}

float RayCastClosestCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) {
	hit = true;
    this->point = point;
    this->normal = normal;
	return fraction;
}
