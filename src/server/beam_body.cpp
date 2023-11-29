#include "box2d/box2d.h"
#include "beam_body.h"
#include <stdio.h>

BeamBody::BeamBody(b2World* world, const float pos_x, const float pos_y): world(world), pos_x(pos_x), pos_y(pos_y) {
	b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(pos_x, pos_y);
    body = world->CreateBody(&bodyDef);
    
    b2PolygonShape polygonShape;
    float angleInRadians = static_cast<float>(angle) * b2_pi / 180.0f;
    polygonShape.SetAsBox(width, height, b2Vec2(0, 0), angleInRadians);
  
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    
    body->CreateFixture(&fixtureDef);
}

BeamBody::BeamBody(b2World* world, const float pos_x, const float pos_y, int angle, float length) : 
                world(world), pos_x(pos_x), pos_y(pos_y), angle(angle), width(length) {
	b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(pos_x, pos_y);
    body = world->CreateBody(&bodyDef);
    
    b2PolygonShape polygonShape;
    float angleInRadians = static_cast<float>(angle) * b2_pi / 180.0f;
    polygonShape.SetAsBox(width, height, b2Vec2(0, 0), angleInRadians);
  
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    
    body->CreateFixture(&fixtureDef);

    /*polygonShape.SetAsBox(width, width, b2Vec2(0, 0), angleInRadians);
    fixtureDef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixtureDef);
    b2FixtureUserData data = footSensorFixture->GetUserData();
    data.pointer = (uintptr_t)3;*/
}
