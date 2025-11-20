#include "explosion_callback.h"
#include <iostream>

ExplosionCallback::ExplosionCallback(int num, Explosion& expl): ray_number(num), explosion(expl) {}

float ExplosionCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) {
    auto* data = reinterpret_cast<UserData*>(fixture->GetBody()->GetUserData().pointer);
    if (data) {
        auto* body = reinterpret_cast<Body*>(data->pointer);
        float intersect_val = body->explosion_intersect_value(fraction);
        //std::cout << "Callback n° " << ray_number << " con cuerpo de tipo: " << body->get_type() << "Con valor de interseccion: " << intersect_val << std::endl;
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
            body.body->apply_explosion(body.point, body.normal);
        }
    }
}

float ExplosionCallback::get_ray_fraction() {
    return smallest_intersection;
}
