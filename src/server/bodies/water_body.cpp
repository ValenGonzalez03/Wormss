#include "box2d/box2d.h"
#include "water_body.h"
#include <stdio.h>

WaterBody::WaterBody(b2World* world, float pos_x, float pos_y, float width, float height) : Body(world, pos_x, pos_y, 0, width, height, 1, 1) {
	b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(pos_x, pos_y);
    body = world->CreateBody(&bodyDef);
    
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(width/2, height/2, b2Vec2(0, 0), 0);
  
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    
    body->CreateFixture(&fixtureDef);
}

void WaterBody::start_contact_with(Body* another_body) { }

void WaterBody::start_contact_with(WormBody* worm) { }
	
void WaterBody::end_contact_with(Body* another_body) { }
