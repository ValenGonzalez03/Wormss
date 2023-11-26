#include "contact_listener.h"
#include "worm_body.h"

void ContactListener::BeginContact(b2Contact* contact) {
	//check if fixture A was a ball
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
      
      
    b2BodyUserData& dataA = bodyA->GetUserData();
	WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
	if(wormA) {
		wormA->start_contact();
	}
		
	b2BodyUserData& dataB = bodyB->GetUserData();
	WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
	if(wormB) {
		wormB->start_contact();
	}
}
  
void ContactListener::EndContact(b2Contact* contact) {
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
      
      
    b2BodyUserData& dataA = bodyA->GetUserData();
	WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
	if(wormA) {
		wormA->end_contact();
	}
		
	b2BodyUserData& dataB = bodyB->GetUserData();
	WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
	if(wormB) {
		wormB->end_contact();
	}
}
