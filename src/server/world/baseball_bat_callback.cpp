#include "baseball_bat_callback.h"

#define BAT_POWER 10

BaseballBatCallback::BaseballBatCallback(b2Vec2 batter_pos, UserData* attacker) :
    attacker_pos(batter_pos), attacker(attacker) {}

float BaseballBatCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal,
                                         float fraction) {
  auto* data = reinterpret_cast<UserData*>(fixture->GetBody()->GetUserData().pointer);
  if (data && data != attacker) {
    auto* body = reinterpret_cast<Body*>(data->pointer);
    b2Vec2 attack_dir = point - attacker_pos;
    attack_dir.Normalize();
    body->apply_impulse(BAT_POWER * attack_dir, point);
  }
  return 1;
}
