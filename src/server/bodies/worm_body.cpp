#include "worm_body.h"
#include "grenade_body.h"
#include "missile_body.h"
#include "box2d/box2d.h"
#include "../../common/game_constants.h"
#include <stdio.h>

const float delta_angle = static_cast<float>(1) * b2_pi / 180.0f;

WormBody::WormBody(BodyBasicData basic_data, BodyAdvData adv_data, int health, float vel, b2World* world) :
    DynamicBody(basic_data, adv_data, WORM, world), health(health), vel(vel) {
  body->SetFixedRotation(true);

  // // Sensor para detectar si el gusano esta tocando el suelo
  b2PolygonShape polygonShape;
  b2FixtureDef fixtureDef;
  polygonShape.SetAsBox((WORM_WIDTH / 1.2) / 2, 0.2 / 2, b2Vec2(0, -WORM_HEIGHT / 2), 0);
  fixtureDef.shape = &polygonShape;
  fixtureDef.isSensor = true;
  body->CreateFixture(&fixtureDef);
}

void WormBody::update() {
  check_boundaries();

  // std::cout << "jump_timeout: " << jump_timeout << std::endl;
  if (jump_timeout > 0) {
    jump_timeout--;
  }
  if (state == MOVING) {
    if (direction == LEFT) {
      move_left();
    } else {
      move_right();
    }
  } else if (state == AIMING) {
    // std::cout << aiming_angle << std::endl;
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



//////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// METODOS DE CONTROL DEL GUSANO /////////////////////////////////

void WormBody::start_moving(const uint8_t& dir) {
  if (state == IDLE || JUMPING) {
    state = MOVING;
    direction = dir;
  }
}

void WormBody::move_left() { apply_horizontal_impulse(-vel); }

void WormBody::move_right() { apply_horizontal_impulse(vel); }

void WormBody::stop_moving() { state = IDLE; }


void WormBody::jump(const uint8_t& dir, const uint8_t& jump_type) {
  if (num_foot_contacts <= 0 || jump_timeout > 0)
    return;
  state = JUMPING;
  // direction = dir;
  if (jump_type == JUMP_BACKWARD) {
    jump_backward();
  } else {
    jump_forward();
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

void WormBody::jump_backward() {
  apply_vertical_impulse(jump_vel_backward);
  if (direction == LEFT) {
    apply_horizontal_impulse(vel);
  } else {
    apply_horizontal_impulse(-vel);
  }
}


void WormBody::start_aiming(const uint8_t& dir) {
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


void WormBody::change_weapon(WeaponType weapon) { current_weapon = weapon; }

///////////////////////////////// METODOS DE CONTROL DEL GUSANO /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// METODOS DE ARMAS Y EXPLOSIONES /////////////////////////////////

b2Vec2 WormBody::calculate_projectile_launch_position(float proj_width, float proj_height, float offset_x,
                                                      float offset_y) {
  b2Vec2 pos = body->GetPosition();
  uint8_t worm_dir = get_direction();
  float missile_dist_x = (worm_dir == RIGHT ? 1 : -1) * ((WORM_WIDTH / 2) + (proj_width / 2) + offset_x);
  float adjusted_pos_x = pos.x + missile_dist_x * cos(aiming_angle);

  float missile_dist_y = ((WORM_HEIGHT / 2) + (proj_height / 2) + offset_y);
  float adjusted_pos_y = pos.y + missile_dist_y * sin(aiming_angle);

  return b2Vec2(adjusted_pos_x, adjusted_pos_y);
}

ExplodableAttr WormBody::attack_projectile(b2Vec2 proj_pos, uint8_t proj_id) {
  uint8_t worm_dir = get_direction();
  float final_angle = (worm_dir == RIGHT ? aiming_angle : -aiming_angle);
  float pos_x = proj_pos.x;
  float pos_y = proj_pos.y;
  return ExplodableAttr{proj_id, WORM, pos_x, pos_y, final_angle, worm_dir};
}

void WormBody::set_worm_to_attack() { state = ATTACKING; }

float WormBody::explosion_intersect_value(float fraction) { return 1; }

void WormBody::update_explosion_ray_contact(const b2Vec2& point, const b2Vec2& center_expl, float fraction) {
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
  auto final_impulse_dir = (1 / static_cast<float>(num_ray_contacts)) * impulse_dir;
  auto final_apply_point = (1 / static_cast<float>(num_ray_contacts)) * apply_point;

  std::cout << "num_ray_contacts: " << num_ray_contacts << std::endl;
  std::cout << "final_impulse_dir: (" << final_impulse_dir.x << ", " << final_impulse_dir.y << ")" << std::endl;
  std::cout << std::endl;

  if (fraction_force <= 0.0f) {
    fraction_force = 0.01f;
  }

  auto body_explosion_info = BodyExplosionInfo{final_apply_point, final_impulse_dir, fraction_force};

  num_ray_contacts = 0;
  impulse_dir = b2Vec2(0, 0);
  apply_point = b2Vec2(0, 0);
  fraction_force = 2.0f;

  return body_explosion_info;
}

void WormBody::teleport(float pos_x, float pos_y) {
  body->SetTransform(b2Vec2(pos_x, pos_y), 0);
  body->SetAwake(true);
}

///////////////////////////////// METODOS DE ARMAS Y EXPLOSIONES /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// GETTERS ///////////////////////////////////////////

BODY_TYPES WormBody::get_type() { return WORM; }

WormState WormBody::get_state() { return state; }

WeaponType WormBody::get_weapon_selected() { return current_weapon; }

uint8_t WormBody::get_direction() { return direction; }

float WormBody::get_aiming_angle() { return aiming_angle; }

////////////////////////////////////////// GETTERS ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// COLISIONES ///////////////////////////////////////////

void WormBody::touch_worm(WormBody* worm) { /* NADA */ }
void WormBody::touch_beam(BeamBody* beam) { hit_a_surface(); }
void WormBody::touch_missile(MissileBody* missile) { /* NADA */ }
void WormBody::touch_grenade(GrenadeBody* grenade) { /* NADA */ }

void WormBody::stop_touching_worm(WormBody* worm) { /* NADA */ }
void WormBody::stop_touching_beam(BeamBody* beam) { move_away_from_surface(); }
void WormBody::stop_touching_missile(MissileBody* missile) { /* NADA */ }
void WormBody::stop_touching_grenade(GrenadeBody* grenade) { /* NADA */ }

void WormBody::hit_a_surface() {
  num_foot_contacts++;
  if (num_foot_contacts == 1) {
    state = IDLE;
  }
  jump_timeout = 30;
}

void WormBody::move_away_from_surface() {
  num_foot_contacts--;
  if (num_foot_contacts == 0) {
    state = JUMPING;
  }
}

///////////////////////////////////////// COLISIONES ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////



WormBody::~WormBody() { free(reinterpret_cast<UserData*>(body->GetUserData().pointer)); }
