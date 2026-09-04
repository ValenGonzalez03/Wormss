#include "body.h"
#include "box2d/box2d.h"

Body::Body(const BodyBasicData& basic_data, const BodyAdvData& adv_data, BODY_TYPES type,
           b2BodyType body_type, b2World* world) :
    width(basic_data.width), height(basic_data.height), angle(basic_data.angle),
    id(basic_data.id) {
  b2BodyDef bodyDef;
  bodyDef.type = body_type;
  bodyDef.position.Set(basic_data.pos_x, basic_data.pos_y);
  // std::cout << "Creating body of type " << type << " with angle " << angle << std::endl;
  bodyDef.angle = basic_data.angle;
  body = world->CreateBody(&bodyDef);


  b2PolygonShape polygonShape;
  polygonShape.SetAsBox(width / 2, height / 2);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &polygonShape;
  fixtureDef.density = adv_data.density;
  fixtureDef.friction = adv_data.friction;
  fixtureDef.filter.categoryBits = adv_data.category_bits;
  fixtureDef.filter.maskBits = adv_data.mask_bits;

  body->CreateFixture(&fixtureDef);

  UserData* data = new UserData{type, this};
  body->GetUserData().pointer = reinterpret_cast<uintptr_t>(data);
}

void Body::start_contact_with(Body* other, b2Fixture self_fixt) {
  auto type = other->get_type();
  switch (type) {
    case WORM: {
      start_contact_with_worm(self_fixt);
      break;
    }
    case BEAM: {
      start_contact_with_beam(self_fixt);
      break;
    }
    case MISSILE: {
      start_contact_with_missile(self_fixt);
      break;
    }
    case GRENADE_BODY: {
      start_contact_with_grenade(self_fixt);
      break;
    }
    case WATER: {
      start_contact_with_water(self_fixt);
      break;
    }

    default:
      break;
  }
}

void Body::end_contact_with(Body* other, b2Fixture self_fixt) {
  switch (other->get_type()) {
    case WORM: {
      end_contact_with_worm(self_fixt);
      break;
    }
    case BEAM: {
      end_contact_with_beam(self_fixt);
      break;
    }
    case GRENADE_BODY: {
      end_contact_with_grenade(self_fixt);
      break;
    }

    default:
      break;
  }
}

void Body::take_damage(int amount) {
  // Por defecto no hace nada. Las subclases pueden sobreescribir este método
}

bool Body::check_boundaries() {
  bool exceeded_horizontal_bounds = (get_pos_x() < 0) || (get_pos_x() > WORLD_WIDTH);
  bool exceeded_vertical_bounds = (get_pos_y() < 0) || (get_pos_y() > WORLD_HEIGHT);
  return exceeded_horizontal_bounds || exceeded_vertical_bounds;
  // if ((get_pos_x() < 0) || (get_pos_x() > 25)) {  // Excedio limite horizontal
  //   body->SetTransform(
  //       b2Vec2(5, 25),
  //       0);  // Por ahora solo fuerzo a que reaparezca mas arriba y a la derecha
  // }
  // if ((get_pos_y() < 0) || (get_pos_y() > WORLD_HEIGHT)) {  // Excedio limite vertical
  //   body->SetTransform(b2Vec2(get_pos_x(), 30),
  //                      0);  // Por ahora solo fuerzo a que reaparezca 30 metros mas arriba
  // }
}

bool Body::is_affected_by_explosions() { return affected_by_explosions; }

void Body::apply_impulse(const b2Vec2& impulse, const b2Vec2& point) {
  body->ApplyLinearImpulse(impulse, point, true);
}

uint8_t Body::get_id() { return id; }

float Body::get_pos_x() { return body->GetPosition().x; }

float Body::get_pos_y() { return body->GetPosition().y; }

float Body::get_width() { return width; }

float Body::get_height() { return height; }

float Body::get_angle() { return body->GetAngle(); }

UserData* Body::get_user_data() {
  return reinterpret_cast<UserData*>(body->GetUserData().pointer);
}
