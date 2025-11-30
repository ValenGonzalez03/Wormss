#include "trajectory_missile_callback.h"

MissileCallback::MissileCallback() {
}

float MissileCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) {
    auto* data = reinterpret_cast<UserData*>(fixture->GetBody()->GetUserData().pointer);
    if (data) {
        auto* body = reinterpret_cast<Body*>(data->pointer);
        if (body->get_type() == BEAM) {
            hit_a_wall = true;
            if (fraction < min_fraction) {
                min_fraction = fraction;
                hit_point = point;
            }
        }
    }
    return 1;
}

bool MissileCallback::did_hit_wall() {
    return hit_a_wall;
}

b2Vec2 MissileCallback::get_hit_point() {
    return hit_point;
}
