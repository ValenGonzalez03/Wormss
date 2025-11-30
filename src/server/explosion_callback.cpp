#include "explosion_callback.h"
#include <iostream>

ExplosionCallback::ExplosionCallback(int num, Explosion& expl): ray_number(num), explosion(expl) {}

float ExplosionCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) {
    auto* data = reinterpret_cast<UserData*>(fixture->GetBody()->GetUserData().pointer);
    if (data) {
        auto* body = reinterpret_cast<Body*>(data->pointer);
        float intersect_val = body->explosion_intersect_value(fraction);
        if (intersect_val < smallest_intersection ) {
            smallest_intersection = intersect_val;
        }
        
        BodyRayInfo body_info {body, point, normal, fraction};
        bodies.push_back(body_info);
        return intersect_val;
    }
	return 1;
}

void ExplosionCallback::evaluate_contact_for_bodies() {
    for (auto body : bodies) {
        if (smallest_intersection >= body.fraction) {
            explosion.try_add_affected_body(body.body);
            b2Vec2 center_explosion = explosion.get_center();
            body.body->update_explosion_ray_contact(body.point, center_explosion, body.fraction);
        }
    }
}

float ExplosionCallback::get_ray_fraction() {
    return smallest_intersection;
}
