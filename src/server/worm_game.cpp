#include "worm_game.h"
#include "box2d/box2d.h"
#include <stdio.h>

WormGame::WormGame(b2World *world, float pos_x, float pos_y)
    : world(world), pos_x(pos_x), pos_y(pos_y) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos_x, pos_y);
  bodyDef.angle = angle;
  body = world->CreateBody(&bodyDef);

  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width / 2, height / 2);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;

  body->CreateFixture(&fixtureDef);
}

void WormGame::move_left() {
  b2Vec2 body_vel = body->GetLinearVelocity();
  float desired_vel = -vel;

  float vel_change = desired_vel - body_vel.x;
  float mass = body->GetMass();
  float impulse = mass * vel_change;

  body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
}

void WormGame::move_right() {
  b2Vec2 body_vel = body->GetLinearVelocity();
  float desired_vel = vel;

  float vel_change = desired_vel - body_vel.x;
  float mass = body->GetMass();
  float impulse = mass * vel_change;

  body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
}

b2Vec2 WormGame::get_position() { return body->GetPosition(); }

float WormGame::get_pos_x() { return body->GetPosition().x; }

float WormGame::get_pos_y() { return body->GetPosition().y; }
