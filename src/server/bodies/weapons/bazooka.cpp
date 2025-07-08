#include "box2d/box2d.h"
#include <stdio.h>
#include "bazooka.h"
#include "../worm_body.h"

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

Bazooka::Bazooka(b2World *world, float pos_x, float pos_y)
    : Weapon(world, pos_x, pos_y, true, false, true, false, false, true, 10000, 50, 2, 0, 0), explosion_manager(world) {
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

void Bazooka::shoot() {
	
}
    
void Bazooka::explode() {
	explosion_manager.simulate_explosion(body->GetPosition(), radius, damage);
}

void Bazooka::start_contact_with(Body* another_body) {
	if (another_body->get_type() == WORM) {
		explode();
		reinterpret_cast<WormBody*>(another_body)->take_damage(damage);
		//std::cout << "BAZOOKA CHOCO CON UN GUSANO\n";
	}
};
	
void Bazooka::end_contact_with(Body* another_body) {}

int Bazooka::get_type() {
	return MISSILE;}
