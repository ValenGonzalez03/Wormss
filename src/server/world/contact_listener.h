#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include "box2d/box2d.h"

class ContactListener : public b2ContactListener {
 private:

 public:
  void BeginContact(b2Contact* contact);

  void EndContact(b2Contact* contact);
};

#endif
