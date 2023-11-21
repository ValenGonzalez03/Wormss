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
  direction = LEFT;
  is_moving = true;
  b2Vec2 body_vel = body->GetLinearVelocity();
  float desired_vel = -vel;
  
  float vel_change = desired_vel - body_vel.x;
  float mass = body->GetMass();
  float impulse = mass * vel_change;
  
  body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
}
	
void WormBody::move_right() {
  direction = RIGHT;
  is_moving = true;
  b2Vec2 body_vel = body->GetLinearVelocity();
  float desired_vel = vel;
  
  float vel_change = desired_vel - body_vel.x;
  float mass = body->GetMass();
  float impulse = mass * vel_change;
  
  body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
}


void WormBody::start_moving(const uint8_t &dir) { 
	state = MOVING;
	if (dir == LEFT) {
		is_moving = true;
		direction = LEFT; 
	} else if (dir == RIGHT) {
		is_moving = true;
		direction = RIGHT;
	}
}

void WormBody::stop_moving() { 
	is_moving = false;	// probablemente no sea necesario con el state
	state = STOPPED;
}

b2Vec2 WormBody::get_position() { return body->GetPosition(); }

float WormBody::get_pos_x() { return body->GetPosition().x; }
	
float WormBody::get_pos_y() { return body->GetPosition().y; }

uint8_t WormBody::get_direction() { return direction; }

uint8_t WormBody::get_state() { return state; }

void WormBody::update() {
	if (is_moving) {
		if (direction == LEFT) {
			move_left();
		} else {
			move_right();
		}
	}
}
