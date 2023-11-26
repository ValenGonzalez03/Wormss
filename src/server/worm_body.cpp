#include "worm_body.h"
#include "box2d/box2d.h"
#include <stdio.h>

WormBody::WormBody(b2World* world, float pos_x, float pos_y)
	: world(world), pos_x(pos_x), pos_y(pos_y) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos_x, pos_y);
  bodyDef.angle = angle;
  body = world->CreateBody(&bodyDef);
  
  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width , height);
  
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;
  
  body->CreateFixture(&fixtureDef);
}

void WormBody::move_left() {
  apply_horizontal_impulse(-vel);
}
	
void WormBody::move_right() {
  apply_horizontal_impulse(vel);
}

void WormBody::apply_horizontal_impulse(float desired_vel) {
  b2Vec2 body_vel = body->GetLinearVelocity();
  float vel_change = desired_vel - body_vel.x;
  float mass = body->GetMass();
  float impulse = mass * vel_change;
  
  body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
}

void WormBody::apply_vertical_impulse(float jump_speed) {
  float impulse = body->GetMass() * jump_speed;
  body->ApplyLinearImpulse(b2Vec2(0, impulse), body->GetWorldCenter(), true);
}

void WormBody::start_moving(const uint8_t &dir) { 
	state = WORM_STATES::MOVING;
	direction = dir;
}

void WormBody::stop_moving() { 
	state = WORM_STATES::STOPPED;
}

void WormBody::jump_left() {
  apply_vertical_impulse(-vel);
}

void WormBody::jump_right() {
  apply_vertical_impulse(vel);
}

void WormBody::jump(const uint8_t &dir) {
  state = WORM_STATES::JUMPING;
  direction = dir;
  if (dir == LEFT) {
    jump_left();
  } else {
    jump_right();
  }
}

b2Vec2 WormBody::get_position() { return body->GetPosition(); }

float WormBody::get_pos_x() { return body->GetPosition().x; }

float WormBody::get_pos_y() { return body->GetPosition().y; }

uint8_t WormBody::get_direction() { return direction; }

uint8_t WormBody::get_state() { return state; }

void WormBody::update() {
	if (state == WORM_STATES::MOVING) {
		if (direction == LEFT) {
			move_left();
		} else {
			move_right();
		}
	}
}
