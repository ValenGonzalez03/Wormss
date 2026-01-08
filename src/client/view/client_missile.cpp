#include "client_missile.h"

#include <cmath>
#include <box2d/b2_common.h>

Missile::Missile(int pos_x, int pos_y, int width, int heigth, float angle, uint8_t dir, uint8_t id,
  std::vector<SDL2pp::Texture *> &&textures, SDL2pp::Renderer &rend) 
  :
  pos_x(pos_x), pos_y(pos_y), width(width), height(heigth), angle(angle), direction(dir), id(id),
  texture(textures), renderer(rend)
  {}

void Missile::update(MissileData data) {
  pos_x = convert_meters_to_pixels_x(data.get_pos_x()) - width / 2;
  pos_y = convert_meters_to_pixels_y(data.get_pos_y()) - height / 2;
  angle = data.get_angle();
}

void Missile::render(int frame) {

  texture[8]->SetAlphaMod(255);
  float angle_deg =  angle * (180.0f / b2_pi);
  SDL_RendererFlip flip = (direction == RIGHT ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
  renderer.Copy(*(texture[8]), SDL2pp::NullOpt, SDL2pp::Rect(pos_x, pos_y, width, height), -angle_deg, SDL2pp::NullOpt, flip);

  if (std::getenv("DEBUG") != NULL) {
    SDL2pp::Rect box(pos_x, pos_y, width, height);
  
    SDL2pp::Color c(0, 255, 0);
    renderer.SetDrawColor(c);

    DrawRotatedRect(renderer, width, height, pos_x, pos_y, -angle);
    //renderer.DrawRect(box);
  }

}

void Missile::DrawRotatedRect(SDL2pp::Renderer& renderer, int width, int height, int pos_x, int pos_y, float angle) {
  int hw = (width * 0.5f);
  int hh = (height * 0.5f);

  float center_x = pos_x + hw;
  float center_y = pos_y + hh;

  float cos_A = std::cos(angle);
  float sin_A = std::sin(angle);

  // Coordenadas locales de los 4 vértices (sentido antihorario)
  SDL2pp::Point verts[4] = {
      {-hw, -hh},
      { hw, -hh},
      { hw,  hh},
      {-hw,  hh}
  };

  SDL2pp::Point worldVerts[4];
    for (int i = 0; i < 4; ++i) {
        worldVerts[i].x = center_x + verts[i].x * cos_A - verts[i].y * sin_A;
        worldVerts[i].y = center_y + verts[i].x * sin_A + verts[i].y * cos_A;
    }

    for (int i = 0; i < 4; ++i) {
      int j = (i + 1) % 4;
      renderer.DrawLine(
          static_cast<int>(worldVerts[i].x),
          static_cast<int>(worldVerts[i].y),
          static_cast<int>(worldVerts[j].x),
          static_cast<int>(worldVerts[j].y));
  }
}

int Missile::get_pos_x() {
  return pos_x;
}

int Missile::get_pos_y() {
  return pos_y;
}

uint8_t Missile::get_id() {
  return id;
}