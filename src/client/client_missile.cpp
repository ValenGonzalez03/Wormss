#include "client_missile.h"

#include <cmath>

Missile::Missile(int pos_x, int pos_y, int width, int heigth, float angle, uint8_t id,
  std::vector<SDL2pp::Texture *> &&textures, SDL2pp::Renderer &rend) 
  :
  pos_x(pos_x), pos_y(pos_y), width(width), height(heigth), angle(angle), id(id),
  texture(textures), renderer(rend)
  {}

void Missile::update(MissileData data) {
  pos_x = convert_meters_to_pixels_x(data.get_pos_x()) - width / 2;
  pos_y = convert_meters_to_pixels_y(data.get_pos_y()) - height / 2;
  angle = data.get_angle();
}

void Missile::render(int frame) {

  texture[8]->SetAlphaMod(255);

  renderer.Copy(*(texture[8]), SDL2pp::NullOpt, // Size
    SDL2pp::Rect(pos_x, pos_y, width, height), -angle,
    SDL2pp::NullOpt, // rotation center - not needed
    SDL_FLIP_NONE);

  if (std::getenv("DEBUG") != NULL) {
    SDL2pp::Rect box(pos_x, pos_y, width, height);
  
    SDL2pp::Color c(0, 255, 0);
    renderer.SetDrawColor(c);
    renderer.DrawRect(box);
  }

}

uint8_t Missile::get_id() {
  return id;
}