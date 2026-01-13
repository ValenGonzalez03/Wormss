#include "worm_body.h"
#include "grenade_body.h"
#include "missile_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

WormBody::WormBody(BodyBasicData basic_data, BodyAdvData adv_data, int health, float vel, b2World* world)
    : DynamicBody(basic_data, adv_data, WORM, world), health(health), vel(vel) {
  body->SetFixedRotation(true);
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
  if (state == IDLE || JUMPING)  {
    state = MOVING;
    direction = dir;
  }
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
  if (state == IDLE) {
    state = AIMING;
    aim_direction = dir;
  }
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

void WormBody::change_weapon(WeaponType weapon) {
  current_weapon = weapon;
}

ExplodableAttr WormBody::attack_projectile(b2Vec2 proj_pos, uint8_t proj_id) {
  uint8_t worm_dir = get_direction();
  float final_angle = (worm_dir == RIGHT ? aiming_angle : -aiming_angle);
  float pos_x = proj_pos.x;
  float pos_y = proj_pos.y;
  return ExplodableAttr {proj_id, WORM, pos_x, pos_y, final_angle, worm_dir};
}

b2Vec2 WormBody::calculate_projectile_launch_position(float proj_width, float proj_height, float offset_x, float offset_y) {
  b2Vec2 pos = body->GetPosition();
  uint8_t worm_dir = get_direction();
  float missile_dist_x = (worm_dir == RIGHT ? 1 : -1) * ((WORM_WIDTH / 2) + (proj_width / 2) + offset_x);
  float adjusted_pos_x = pos.x  + missile_dist_x * cos(aiming_angle);

  float missile_dist_y = ((WORM_HEIGHT / 2) + (proj_height / 2) + offset_y);
  float adjusted_pos_y = pos.y  + missile_dist_y * sin(aiming_angle);

  return b2Vec2(adjusted_pos_x, adjusted_pos_y);
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

WeaponType WormBody::get_weapon_selected() {
  return current_weapon;
}

void WormBody::set_worm_to_attack() {
  state = ATTACKING;
}

void WormBody::update() {
  check_boundaries();

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
  } else if (state == ATTACKING) {
    frames_attacking++;
    if (frames_attacking >= 60) {
      frames_attacking = 0;
      state = IDLE;
    }
  }
}

float WormBody::explosion_intersect_value(float fraction) {
  return 1;
}

void WormBody::update_explosion_ray_contact(b2Vec2& point, b2Vec2& center_expl, float fraction) {
  num_ray_contacts++;
  b2Vec2 ray_direction = point - center_expl;
  ray_direction.Normalize();
  impulse_dir += ray_direction;
  apply_point += point;
  if (fraction < fraction_force) {
    fraction_force = fraction;
  }
  // std::cout << "ray_direction added: (" << ray_direction.x << ", " << ray_direction.y << ")" << std::endl;
  // std::cout << "fraction of ray: " << fraction << std::endl;
  // std::cout << "fraction_force updated to: " << fraction_force << std::endl;
  // std::cout << "new impulse_dir: (" << impulse_dir.x << ", " << impulse_dir.y << ")" << std::endl;
  // std::cout << std::endl;
}

BodyExplosionInfo WormBody::get_explosion_info() {
  auto final_impulse_dir =  (1 / static_cast<float>(num_ray_contacts)) * impulse_dir;
  auto final_apply_point =  (1 / static_cast<float>(num_ray_contacts)) * apply_point;

  std::cout << "num_ray_contacts: " << num_ray_contacts << std::endl;
  std::cout << "final_impulse_dir: (" << final_impulse_dir.x << ", " << final_impulse_dir.y << ")" << std::endl;
  std::cout << std::endl;
  //std::cout << "final_apply_point: (" << final_apply_point.x << ", " << final_apply_point.y << ")" << std::endl;

  if (fraction_force <= 0.0f) {
    fraction_force = 0.01f;
  }

  auto body_explosion_info = BodyExplosionInfo {final_apply_point, final_impulse_dir, fraction_force};

  num_ray_contacts = 0;
  impulse_dir = b2Vec2(0,0);
  apply_point = b2Vec2(0,0);
  fraction_force = 2.0f;

  return body_explosion_info;
}

bool WormBody::is_facing_left() { return (direction == LEFT); }

bool WormBody::is_facing_right() { return (direction == RIGHT); }

bool WormBody::is_stopped() { return (IDLE); } // ??

bool WormBody::is_inactive() { return (state == IDLE); } // ??

void WormBody::touch_beam(BeamBody* beam) { /* NADA */ }
void WormBody::touch_worm(WormBody* worm) { /* NADA */ }
void WormBody::touch_missile(MissileBody* missile) { missile->explode(); }
void WormBody::touch_grenade(GrenadeBody* grenade) { /* NADA */ }

void WormBody::stop_touching_worm(WormBody* worm) { /* NADA */ }
void WormBody::stop_touching_beam(BeamBody* beam) { /* NADA */ }
void WormBody::stop_touching_missile(MissileBody* missile) { /* NADA */ }
void WormBody::stop_touching_grenade(GrenadeBody* grenade) { /* NADA */ }

void WormBody::hit_a_surface() { 
  state = IDLE;
}

void WormBody::move_away_from_surface() {
  state = JUMPING;
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

BODY_TYPES WormBody::get_type() { return WORM; }

WormBody::~WormBody() {
  free(reinterpret_cast<UserData*>(body->GetUserData().pointer));
}
