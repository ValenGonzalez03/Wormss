#include "box2d/box2d.h"
#include "beam_body.h"
#include <stdio.h>

/*
BeamBody::BeamBody(b2World* world, float pos_x, float pos_y) : Body(world, pos_x, pos_y, 0, 6, 0.8, 1, 0.5) {
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
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}
*/

BeamBody::BeamBody(b2World* world, float pos_x, float pos_y, int angle, float width) : 
                Body(world, pos_x, pos_y, angle, width, 0.8, 1, 0.5) {
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
    body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    /*polygonShape.SetAsBox(width, width, b2Vec2(0, 0), angleInRadians);
    fixtureDef.isSensor = true;
    b2Fixture* footSensorFixture = body->CreateFixture(&fixtureDef);
    b2FixtureUserData data = footSensorFixture->GetUserData();
    data.pointer = (uintptr_t)3;*/
}

void BeamBody::start_contact_with(Body* another_body) { 
	if (another_body->get_type() == WORM) {}
}

void BeamBody::start_contact_with(WormBody* worm) { }
	
void BeamBody::end_contact_with(Body* another_body) { }

int BeamBody::get_type() {return BEAM;}
