#include "worm_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

WormBody::WormBody(b2World *world, float pos_x, float pos_y, float vel, int health, uint8_t id)
    : Body(world, pos_x, pos_y, 0, WORM_WIDTH, WORM_HEIGHT, 1, 0.2), vel(vel), id(id), health(health) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos_x, pos_y);
  bodyDef.angle = angle;
  body = world->CreateBody(&bodyDef);

  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width / 2, height / 2);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;

  body->CreateFixture(&fixtureDef);
  body->SetFixedRotation(true);

  UserData* data = new UserData {BODY_TYPES::WORM, this};
  body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

  // // sensor
  // polygonShape.SetAsBox(0.3, 0.6, b2Vec2(pos_x, -0.5), 0);
  // fixtureDef.isSensor = true;
  // b2Fixture *footSensorFixture = body->CreateFixture(&fixtureDef);
  // footSensorFixture->GetUserData().pointer = (uintptr_t)3;
}

void WormBody::move_left() { apply_horizontal_impulse(-vel); }

void WormBody::move_right() { apply_horizontal_impulse(vel); }

void WormBody::apply_horizontal_impulse(float desired_vel) {
  b2Vec2 body_vel = body->GetLinearVelocity();
  float vel_change = desired_vel - body_vel.x;
  float mass = body->GetMass();
  float impulse = mass * vel_change;

  body->ApplyLinearImpulse(b2Vec2(impulse, 0), body->GetWorldCenter(), true);
}

void WormBody::apply_vertical_impulse(float jump_speed) {
  float impulse = body->GetMass() * jump_speed;
  body->ApplyLinearImpulse(b2Vec2(0, impulse), body->GetWorldCenter(), true);
}

void WormBody::start_moving(const uint8_t &dir) {
  if (state == JUMPING) return;
  state = MOVING;
  direction = dir;
}

void WormBody::stop_moving() {
  state = IDLE; 
}

void WormBody::jump_backward() {
  apply_vertical_impulse(jump_vel_backward); 
  if (direction == LEFT) {
	  apply_horizontal_impulse(vel);
  } else {
	  apply_horizontal_impulse(-vel);
  }
}

void WormBody::jump_forward() { 
  apply_vertical_impulse(jump_vel_forward); 
  if (direction == LEFT) {
	  apply_horizontal_impulse(-(vel + 0.1));
  } else {
	  apply_horizontal_impulse(vel + 0.1);
  }
}

void WormBody::jump(const uint8_t &dir, const uint8_t &jump_type) {
  if (state == JUMPING)
    return;
  state = JUMPING;
  //direction = dir;
  if (jump_type == JUMP_BACKWARD) {
    jump_backward();
  } else {
    jump_forward();
  }
}

void WormBody::start_aiming(const uint8_t &dir) {
  state = AIMING;
  aim_direction = dir;
}

void WormBody::aim_up() {
  if (aiming_angle + delta_angle <= (b2_pi / 2.0)) {
    aiming_angle += delta_angle;
  } else {
    aiming_angle = (b2_pi / 2.0);
  }
}

void WormBody::aim_down() {
  if (aiming_angle - delta_angle >= -(b2_pi / 2.0)) {
    aiming_angle -= delta_angle;
  } else {
    aiming_angle = -(b2_pi / 2.0);
  }
}

void WormBody::stop_aiming() { state = IDLE; }

MissileBody* WormBody::shoot(float initial_force, int missile_id) {
  b2Vec2 pos = body->GetPosition();
  MissileBody *missile = new MissileBody(world, pos.x, pos.y, initial_force, aiming_angle, missile_id);
  return missile;
}

void WormBody::teleport(float pos_x, float pos_y) {
  body->SetTransform( b2Vec2(pos_x, pos_y), 0);
  body->SetAwake(true);
}

uint8_t WormBody::get_id() { return id; }

b2Vec2 WormBody::get_position() { return body->GetPosition(); }

float WormBody::get_pos_x() { return body->GetPosition().x; }

float WormBody::get_pos_y() { return body->GetPosition().y; }

uint8_t WormBody::get_direction() { return direction; }

WormState WormBody::get_state() { return state; }

float WormBody::get_aiming_angle() { return aiming_angle; }

void WormBody::update() {
  if (has_exceeded_width_limit()) {
    body->SetTransform(b2Vec2(15, 25), 0); // Por ahora solo fuerzo a que reaparezca mas arriba y a la derecha
  }
  if (has_exceeded_height_limit()) {
    body->SetTransform(b2Vec2(get_pos_x(), 25), 0); // Por ahora solo fuerzo a que reaparezca 25 metros mas arriba
  }
  if (state == MOVING) {
    if (direction == LEFT) {
      move_left();
    } else {
      move_right();
    }
  } else if (state == AIMING) {
    std::cout << aiming_angle << std::endl;
    if (aim_direction == UP) {
      aim_up();
    } else {
      aim_down();
    }
  }
}

bool WormBody::is_facing_left() { return (direction == LEFT); }

bool WormBody::is_facing_right() { return (direction == RIGHT); }

bool WormBody::is_stopped() { return (IDLE); } // ??

bool WormBody::is_inactive() { return (state == IDLE); } // ??

bool WormBody::has_exceeded_width_limit() { return get_pos_x() < 0; }

bool WormBody::has_exceeded_height_limit() { return get_pos_y() < 0; }

void WormBody::start_contact_with(Body *other) {
  if (other->get_type() == BEAM) {
    state = IDLE;
  }

  /*
  if (other->get_type() == WORM) {
		//std::cout << "GUSANO CHOCO CON UN GUSANO\n";
		reinterpret_cast<WormBody*>(other)->start_contact_with(this);
  }
  
  if (other->get_type() == WATER) {
		std::cout << "GUSANO CHOCO CON AGUA\n";
		die();
  }
  
  if (other->get_type() == BAZOOKA) {
		std::cout << "GUSANO CHOCO CON MUNICION DE BAZOOKA\n";
		//take_damage(another_body->get_damage());
  }
  */
  
}

void WormBody::start_contact_with(WormBody *another_worm) {
  if (is_inactive()) {
    if (another_worm->is_inactive())
      return;
    another_worm->start_contact_with(this);
  } else {
    if (is_facing_left()) {
      apply_horizontal_impulse(vel);
    } else {
      apply_horizontal_impulse(-vel);
    }
  }
}

void WormBody::start_contact_with(WaterBody *water) {
	//die();
}

void WormBody::end_contact_with(Body *another_body) {}

void WormBody::hit_a_surface() { 
  num_foot_contacts++;
  if (not is_inactive()) {
	  state = IDLE;
  } 
}

void WormBody::move_away_from_surface() {
  num_foot_contacts--;
  if (num_foot_contacts == 0) {
	  state = JUMPING;
  }
}

void WormBody::show_vel_and_health() {
  std::cout << "worm vel: " << vel << std::endl;
  std::cout << "worm health: " << health << std::endl; 
}

void WormBody::take_damage(int damage) {
  health -= damage;
}

void WormBody::die() {
  //state = WORM_STATES::DEATH;
}

// POR AHORA DE PRUEBA
void WormBody::start_contact() { m_contacting = true; }
void WormBody::end_contact() { m_contacting = false; }

int WormBody::get_type() {
	return WORM;}

WormBody::~WormBody() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
