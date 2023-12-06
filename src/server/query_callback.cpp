//Fuente: https://www.iforce2d.net/b2dtut/world-querying

#include "query_callback.h"

bool QueryCallback::ReportFixture(b2Fixture* fixture) {
	foundBodies.push_back(fixture->GetBody()); 
	return true;
}
