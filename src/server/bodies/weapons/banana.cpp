#include "box2d/box2d.h"
#include <stdio.h>
#include "banana.h"

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

Banana::Banana(b2World *world, float pos_x, float pos_y)
    : Weapon(world, pos_x, pos_y, true, false, true, true, false, false, 5, 70, 4, 0, 0) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos_x, pos_y);
  bodyDef.angle = angle;
  body = world->CreateBody(&bodyDef);

  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width/2, height/2);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;

  body->CreateFixture(&fixtureDef);
  body->SetFixedRotation(true);
  body->GetUserData().pointer = (uintptr_t)this;
}

void Banana::start_contact_with(Body* another_body) {
	if (another_body->get_type() == WORM) {
	}
};
	
void Banana::end_contact_with(Body* another_body) {}

int Banana::get_type() {
	return BANANA;}
