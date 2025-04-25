#include "box2d/box2d.h"
#include "beam_body.h"
#include "../../common/game_constants.h"
#include <stdio.h>

BeamBody::BeamBody(b2World* world, float pos_x, float pos_y, int angle, float width) : 
                Body(world, pos_x, pos_y, angle, width, BEAM_HEIGHT, 1.0f, 0.5f) {
	b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(pos_x, pos_y);
    body = world->CreateBody(&bodyDef);
    
    b2PolygonShape polygonShape;
    float angleInRadians = static_cast<float>(angle) * b2_pi / 180.0f;
    polygonShape.SetAsBox(width / 2.0f, height / 2.0f, b2Vec2(0,0), angleInRadians);
  
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
