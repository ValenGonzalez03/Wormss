#ifndef BASEBALL_BAT_CALLBACK_H
#define BASEBALL_BAT_CALLBACK_H

#include "../bodies/body.h"

class BaseballBatCallback : public b2RayCastCallback {
 private:
  UserData* attacker;
  b2Vec2 attacker_pos;

 public:
  explicit BaseballBatCallback(b2Vec2 batter_pos, UserData* attacker);

  float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
};

#endif