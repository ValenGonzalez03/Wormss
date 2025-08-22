#include "missile_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

MissileBody::MissileBody(b2World *world, float pos_x, float pos_y, float inicial_force, float angle, uint8_t dir, uint8_t id)
    : Body(world, pos_x, pos_y, angle, MISSILE_WIDTH, MISSILE_HEIGHT, 1.0f, 0.3f), initial_force(inicial_force), 
      missile_direction(dir), id(id) {
  b2BodyDef bodyDef;
  bodyDef.bullet = true;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos_x, pos_y); // Ajusto la distancia inicial un poco mas adelante para que no choque con el gusano.
  bodyDef.angle = angle;
  body = world->CreateBody(&bodyDef);

  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width / 2, height / 2);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;

  body->CreateFixture(&fixtureDef);

  UserData* data = new UserData {MISSILE, this};
  body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}

void MissileBody::apply_initial_impulse(float aim_angle) {
  float vel_x = cos(aim_angle) * this->initial_force;
  float vel_y = sin(aim_angle) * this->initial_force;
  b2Vec2 dir_vec = (missile_direction == RIGHT ? b2Vec2(vel_x, vel_y) : b2Vec2(-vel_x, vel_y)) ;
  body->ApplyLinearImpulse(dir_vec, body->GetWorldCenter(), true);
}

float MissileBody::explosion_intersect_value(float fraction) {
  return fraction;
}

void MissileBody::apply_explosion(b2Vec2& point, b2Vec2& normal) {
  /* NADA */
}

b2Vec2 MissileBody::get_position() { return body->GetPosition(); }

float MissileBody::get_pos_x() { return body->GetPosition().x; }

float MissileBody::get_pos_y() { return body->GetPosition().y; }

float MissileBody::get_angle() { return body->GetAngle(); }

uint8_t MissileBody::get_direction() { return missile_direction; }

uint8_t MissileBody::get_id() { return id; }

void MissileBody::update() {
  if (has_exceeded_width_limit()) {
    body->SetTransform(b2Vec2(15, 25), 0); // Por ahora solo fuerzo a que reaparezca mas arriba y a la derecha
  }
  if (has_exceeded_height_limit()) {
    body->SetTransform(b2Vec2(get_pos_x(), 25), 0); // Por ahora solo fuerzo a que reaparezca 25 metros mas arriba
  }
}

bool MissileBody::has_exceeded_width_limit() { return get_pos_x() < 0; }

bool MissileBody::has_exceeded_height_limit() { return get_pos_y() < 0; }

void MissileBody::touch_beam(BeamBody* beam) {
  /* NADA */
}

void MissileBody::touch_worm(WormBody* worm) {
  /* NADA */
}

void MissileBody::touch_missile(MissileBody* missile) {
  /* MISIL DEBE EXPLOTAR */
}

void MissileBody::stop_touching_worm(WormBody* worm) {
  /* NADA */
}

void MissileBody::stop_touching_beam(BeamBody* beam) {
  /* NADA */
}

void MissileBody::stop_touching_missile(MissileBody* missile) {
  /* NADA */
}

void MissileBody::explode() {
  exploded = true;
}

bool MissileBody::has_exploded() {
  return exploded;
}

// void MissileBody::start_contact_with(Body *another_body) {
//   std::cout << "MISIL EXPLOTO" << std::endl;
// }
// void MissileBody::end_contact_with(Body *another_body) {}

int MissileBody::get_type() {return MISSILE;}

MissileBody::~MissileBody() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
