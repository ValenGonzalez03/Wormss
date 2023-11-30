#include "contact_listener.h"
#include "bodies/worm_body.h"

#include <iostream>


void ContactListener::BeginContact(b2Contact* contact) {
	
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	
	b2BodyUserData& dataA = fixtureA->GetBody()->GetUserData();
    b2BodyUserData& dataB = fixtureB->GetBody()->GetUserData();
	
	if ( (size_t)fixtureA->GetUserData().pointer == 3 ) {
		WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
		if(wormA) {
			wormA->hit_a_surface();
		}
    }
	
	if ( (size_t)fixtureB->GetUserData().pointer == 3 ) {
		WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
		if(wormB) {
			wormB->hit_a_surface();
		}
    }
    
	Body* body = reinterpret_cast<Body*>(dataA.pointer);
	Body* another_body = reinterpret_cast<Body*>(dataB.pointer);
	
	if(body && another_body) {
		body->start_contact_with(another_body);
	}
}
  
void ContactListener::EndContact(b2Contact* contact) {
	
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	
	b2BodyUserData& dataA = fixtureA->GetBody()->GetUserData();
    b2BodyUserData& dataB = fixtureB->GetBody()->GetUserData();
	
	
	if ( (size_t)fixtureA->GetUserData().pointer == 3 ) {
		WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
		if(wormA) {
			wormA->move_away_from_surface();
		}
    }
	
	if ( (size_t)fixtureB->GetUserData().pointer == 3 ) {
		WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
		if(wormB) {
			wormB->move_away_from_surface();
		}
    }
	
	/*
	b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();
      
    b2BodyUserData& dataA = bodyA->GetUserData();
    b2BodyUserData& dataB = bodyB->GetUserData();
    
	Body* body = reinterpret_cast<Body*>(dataA.pointer);
	Body* another_body = reinterpret_cast<Body*>(dataB.pointer);
	
	if(body) {
		std::cout << "DEJA DE TENER CONTACTO\n";
		//body->end_contact_with(another_body);
	}
	*/
}
