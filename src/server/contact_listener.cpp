#include "contact_listener.h"
#include "bodies/worm_body.h"

#include "bodies/beam_body.h"
#include "bodies/water_body.h"
#include "bodies/body.h"

#include <iostream>


void ContactListener::BeginContact(b2Contact* contact) {
	auto* data_A = reinterpret_cast<UserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    auto* data_B = reinterpret_cast<UserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	if (data_A && data_B) {
		auto* pointer_A = reinterpret_cast<Body*>(data_A->pointer);
		auto* pointer_B = reinterpret_cast<Body*>(data_B->pointer);
		pointer_A->start_contact_with(pointer_B);
		pointer_B->start_contact_with(pointer_A);

	}
	// if (data_A->type == WORM || data_B->type == WORM) {
	// } else if (data_A->type == MISSILE || data_B->type == MISSILE) {
	// 	auto* pointer_A = reinterpret_cast<Body*>(data_A->pointer);
	// 	auto* pointer_B = reinterpret_cast<Body*>(data_B->pointer);
	// 	pointer_A->start_contact_with(pointer_B);
	// 	pointer_B->start_contact_with(pointer_A);
	// }
/*
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
	
	b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();
	
	b2BodyUserData& dataA = bodyA->GetUserData();
    b2BodyUserData& dataB = bodyB->GetUserData();
	
	if ((size_t)fixtureA->GetUserData().pointer == 3) {
		WormBody* wormA = reinterpret_cast<WormBody*>(dataA.pointer);
		if(wormA) {
			wormA->hit_a_surface();
			return;
		}
    }
	
	if ((size_t)fixtureB->GetUserData().pointer == 3) {
		WormBody* wormB = reinterpret_cast<WormBody*>(dataB.pointer);
		if(wormB) {
			wormB->hit_a_surface();
			return;
		}
    }
    
    Body* body = reinterpret_cast<Body*>(dataA.pointer);
	Body* another_body = reinterpret_cast<Body*>(dataB.pointer);
	
	
	if(body && another_body) {
		body->start_contact_with(another_body);
	}
*/
}
  
void ContactListener::EndContact(b2Contact* contact) {
	auto* data_A = reinterpret_cast<UserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    auto* data_B = reinterpret_cast<UserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	
	if (data_A->type == WORM || data_B->type == WORM) {
		auto* pointer_A = reinterpret_cast<Body*>(data_A->pointer);
		auto* pointer_B = reinterpret_cast<Body*>(data_B->pointer);
		pointer_A->end_contact_with(pointer_B);
		pointer_B->end_contact_with(pointer_A);
	}
/*	
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
*/
}
