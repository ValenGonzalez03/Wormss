#ifndef QUERY_CALLBACK_H
#define QUERY_CALLBACK_H

//Fuente: https://www.iforce2d.net/b2dtut/world-querying

#include "box2d/box2d.h"
#include <vector>

class QueryCallback : public b2QueryCallback {
 public:
  std::vector<b2Body*> foundBodies;

  bool ReportFixture(b2Fixture* fixture);
};

#endif
