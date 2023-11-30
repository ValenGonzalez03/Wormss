#include "worm_body.h"
#include "box2d/box2d.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

WormBody::WormBody(b2World *world, float pos_x, float pos_y, uint8_t id)
    : world(world), pos_x(pos_x), pos_y(pos_y), id(id) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos_x, pos_y);
  bodyDef.angle = angle;
  body = world->CreateBody(&bodyDef);

  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width, height);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;

  body->CreateFixture(&fixtureDef);
  body->SetFixedRotation(true);
  body->GetUserData().pointer = (uintptr_t)this;

  // sensor
  polygonShape.SetAsBox(0.3, 0.3, b2Vec2(pos_x, -0.5), 0);
  fixtureDef.isSensor = true;
  b2Fixture *footSensorFixture = body->CreateFixture(&fixtureDef);
  footSensorFixture->GetUserData().pointer = (uintptr_t)3;
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
  state = WORM_STATES::MOVING;
  direction = dir;
}

void WormBody::stop_moving() { state = WORM_STATES::STOPPED; }

void WormBody::jump_left() {
  apply_vertical_impulse(jump_vel); // CAMBIAR A QUE SALTE PARA ATRAS, PERO ASI
                                    // FUNCIONA EL SALTO A LA IZQUIERDA
}

void WormBody::jump_right() { apply_vertical_impulse(jump_vel); }

void WormBody::jump(const uint8_t &dir) {
  // state = WORM_STATES::JUMPING;
  direction = dir;
  if (dir == LEFT) {
    jump_left();
  } else {
    jump_right();
  }
}

void WormBody::start_aiming(const uint8_t &dir) {
  state = WORM_STATES::AIMING;
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

void WormBody::stop_aiming() { state = WORM_STATES::STOPPED; }

uint8_t WormBody::get_id() { return id; }

b2Vec2 WormBody::get_position() { return body->GetPosition(); }

float WormBody::get_pos_x() { return body->GetPosition().x; }

float WormBody::get_pos_y() { return body->GetPosition().y; }

uint8_t WormBody::get_direction() { return direction; }

uint8_t WormBody::get_state() { return state; }

void WormBody::update() {
  if (state == WORM_STATES::MOVING) {
    if (direction == LEFT) {
      move_left();
    } else {
      move_right();
    }
  } else if (state == WORM_STATES::AIMING) {
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

bool WormBody::is_stopped() { return (WORM_STATES::STOPPED); }

void WormBody::start_contact_with(Body *another_body) {
  another_body->start_contact_with(this);
}

void WormBody::start_contact_with(WormBody *another_worm) {
  if (state == WORM_STATES::STOPPED) {
    if (another_worm->is_stopped())
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

void WormBody::end_contact_with(Body *another_body) {}

void WormBody::hit_a_surface() { state = WORM_STATES::STOPPED; }

void WormBody::move_away_from_surface() { state = WORM_STATES::JUMPING; }

// POR AHORA DE PRUEBA
void WormBody::start_contact() { m_contacting = true; }
void WormBody::end_contact() { m_contacting = false; }
