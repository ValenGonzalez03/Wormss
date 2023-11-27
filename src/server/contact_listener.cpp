#include "contact_listener.h"
#include "worm_body.h"

void ContactListener::BeginContact(b2Contact* contact) {
	/*	PARA DETECTAR CUANDO SE TOCA EL SUELO
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	
	if ( (size_t)fixtureA->GetUserData().pointer == 3 ) {
		b2Body* bodyA = fixtureA->GetBody();
		b2BodyUserData& dataA = bodyA->GetUserData();
		WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
		if(wormA) {
			wormA->fall_in_ground();
		}
    }
	
	if ( (size_t)fixtureB->GetUserData().pointer == 3 ) {
		b2Body* bodyB = fixtureB->GetBody();
		b2BodyUserData& dataB = bodyB->GetUserData();
		WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
		if(wormB) {
			wormB->fall_in_ground();
		}
    }
	//b2Body* bodyA = contact->GetFixtureA()->GetBody();
    //b2Body* bodyB = contact->GetFixtureB()->GetBody();
      */
      
    b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
      
    b2BodyUserData& dataA = bodyA->GetUserData();
    b2BodyUserData& dataB = bodyB->GetUserData();
    
	WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
	WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
	
	if(wormA && wormB) {
		wormA->start_contact_with(wormB);
	}
}
  
void ContactListener::EndContact(b2Contact* contact) {
	/*
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	
	if ( (size_t)fixtureA->GetUserData().pointer == 3 ) {
		b2Body* bodyA = fixtureA->GetBody();
		b2BodyUserData& dataA = bodyA->GetUserData();
		WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
		if(wormA) {
			wormA->end_contact();
		}
    }
	
	if ( (size_t)fixtureB->GetUserData().pointer == 3 ) {
		b2Body* bodyB = fixtureA->GetBody();
		b2BodyUserData& dataB = bodyB->GetUserData();
		WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
		if(wormB) {
			wormB->end_contact();
		}
    }
	*/
	
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
    b2Body* bodyB = contact->GetFixtureB()->GetBody();
      
    b2BodyUserData& dataA = bodyA->GetUserData();
    b2BodyUserData& dataB = bodyB->GetUserData();
    
	WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
	WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
	
	if(wormA && wormB) {
		wormA->end_contact_with(wormB);
	}
}
