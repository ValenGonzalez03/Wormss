#ifndef BODY_H
#define BODY_H

class WormBody;

class Body {
private:

public:
  Body();
  
  virtual void start_contact_with(Body* another_body) = 0;
  virtual void start_contact_with(WormBody* worm) = 0;
  //virtual void start_contact_with(BeamBody* b) = 0;
  virtual void end_contact_with(Body* another_body) = 0;
  //virtual void get_entity_type() = 0;
	
  Body(const Body &) = delete;
  Body &operator=(const Body &) = delete;
};

#endif
