#include "contact_listener.h"
#include "../bodies/body.h"
#include "../bodies/worm_body.h"
#include "../bodies/beam_body.h"
#include "../bodies/water_body.h"

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
}
  
void ContactListener::EndContact(b2Contact* contact) {
	auto* data_A = reinterpret_cast<UserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    auto* data_B = reinterpret_cast<UserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	
	if (data_A && data_B) {
		auto* pointer_A = reinterpret_cast<Body*>(data_A->pointer);
		auto* pointer_B = reinterpret_cast<Body*>(data_B->pointer);
		pointer_A->end_contact_with(pointer_B);
		pointer_B->end_contact_with(pointer_A);
	}
}
