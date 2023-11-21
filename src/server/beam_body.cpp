#include "box2d/box2d.h"
#include "beam_body.h"
#include <stdio.h>

BeamBody::BeamBody(b2World* world, const float pos_x, const float pos_y): world(world), pos_x(pos_x), pos_y(pos_y) {
	b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(0, 0);
    body = world->CreateBody(&bodyDef);
    
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(width, height, b2Vec2(0, 0), angle);
  
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.density = density;
    fixtureDef.friction = friction;
    
    body->CreateFixture(&fixtureDef);
}
