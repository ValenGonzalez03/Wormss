#include "missile_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

MissileBody::MissileBody(b2World *world, float pos_x, float pos_y, float inicial_force, float angle, int id)
    : Body(world, pos_x, pos_y, angle, MISSILE_WIDTH, MISSILE_HEIGHT, 1.0f, 0.3f), initial_force(inicial_force), id(id) {
  b2BodyDef bodyDef;
  bodyDef.bullet = true;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(pos_x + (WORM_WIDTH / 2) + 0.3f * cos(angle), pos_y + 1.0f * sin(angle)); // Ajusto la distancia inicial un poco mas adelante para que no choque con el gusano.
  bodyDef.angle = angle; // Por ahora dejo el angulo en 0 por defecto
  body = world->CreateBody(&bodyDef);

  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width / 2, height / 2);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = density;
  fixtureDef.friction = friction;

  body->CreateFixture(&fixtureDef);
 //body->SetFixedRotation(true);

  UserData* data = new UserData {MISSILE, this};
  body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);

}

void MissileBody::apply_initial_impulse(float aim_angle) {
  float vel_x = cos(aim_angle) * this->initial_force;
  float vel_y = sin(aim_angle) * this->initial_force;
  body->ApplyLinearImpulse(b2Vec2(vel_x, vel_y), body->GetWorldCenter(), true);
}

b2Vec2 MissileBody::get_position() { return body->GetPosition(); }

float MissileBody::get_pos_x() { return body->GetPosition().x; }

float MissileBody::get_pos_y() { return body->GetPosition().y; }

float MissileBody::get_angle() { return body->GetAngle(); }

int MissileBody::get_id() { return id; }

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

void MissileBody::start_contact_with(Body *another_body) {}
void MissileBody::end_contact_with(Body *another_body) {}

int MissileBody::get_type() {return MISSILE;}

MissileBody::~MissileBody() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
