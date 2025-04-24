#include "client_worm.h"

#include <cmath>

Worm::Worm(int id, int pos_x, int pos_y, int width, int height, float aim_angle, uint8_t direction, 
  uint8_t state, std::vector<std::vector<SDL2pp::Texture *>> &&textures, SDL2pp::Renderer &rend)
      : id(id), pos_x(pos_x), pos_y(pos_y), width(width), height(height), aim_angle(aim_angle), direction(direction), 
        worm_state(state), textures(textures), renderer(rend) {}

int Worm::get_id() {
  return id;
}

void Worm::update(WormData data) {
  pos_x = convert_meters_to_pixels_x(data.get_pos_x()) - width / 2;
  pos_y = convert_meters_to_pixels_y(data.get_pos_y()) - height / 2;
  id = data.get_player_id();
  worm_state = data.get_state();
  direction = data.get_direction();
  aim_angle = data.get_aim_angle();
}

void Worm::render(int frame, client_state &worm_state) {
  /*
  std::cout << "Posx en m: " << worm.get_position().get_position_x()
            << std::endl;
  std::cout << "Posy en m: " << worm.get_position().get_position_y()
            << std::endl;
  std::cout << "Posx en px: " << pos_x << std::endl;
  std::cout << "Posy en px: " << pos_y << std::endl;
  */
  if (this->worm_state == WORM_STATES::MOVING)
    render_worm_running(frame, worm_state);
  else if (this->worm_state == WORM_STATES::AIMING) {
    render_worm_aiming(frame);
  } else { // worm.get_state() == idle == 0
    render_worm_idle(frame, worm_state);
  }

  if (std::getenv("DEBUG") != NULL) {
    SDL2pp::Rect box(pos_x, pos_y + 3, 19, 25);
  
    SDL2pp::Color c(255, 0, 0);
    renderer.SetDrawColor(c);
    renderer.DrawRect(box);
  }

}

void Worm::render_worm_idle(int frame, client_state &worm_state) {
  worm_state.run_phase = 0;
  SDL_RendererFlip flip = choose_flip_direction();

  auto walk_texture = textures[0];
  //std::cout << "LLEGO?" << std::endl;
  walk_texture.front()->SetAlphaMod(255);
  walk_texture.front()->SetBlendMode(SDL_BLENDMODE_BLEND);

  renderer.Copy(*walk_texture.front(), SDL2pp::NullOpt, // Size
                SDL2pp::Rect(pos_x, pos_y, 21, 28),        // Destination
                0.0,                                       // don't rotate
                SDL2pp::NullOpt, // rotation center - not needed
                flip             // horizontal flip
  );
}

void Worm::render_worm_running(int frame, client_state &worm_state) {
  SDL_RendererFlip flip = choose_flip_direction();

  auto walk_texture = textures[0];
  auto normalized_frame = frame / 40;
  auto frame_position = normalized_frame % walk_texture.size();
  walk_texture[frame_position]->SetBlendMode(SDL_BLENDMODE_BLEND);
  walk_texture[frame_position]->SetAlphaMod(255); // sprite is fully opaque

  renderer.Copy(*walk_texture[frame_position],
                SDL2pp::NullOpt, SDL2pp::Rect(pos_x, pos_y, 21, 28), 0.0,
                SDL2pp::NullOpt, flip);
}

void Worm::render_worm_jumping(int frame, client_state &worm_state) {}

void Worm::render_worm_aiming(int frame) {
  SDL_RendererFlip flip = choose_flip_direction();

  auto aim_texture = textures[1];
  auto normalized_angle = (aim_angle / M_PI_2);
  auto frame_position = 16 + (int)(normalized_angle * ((aim_texture.size() / 2)));
  aim_texture[frame_position]->SetBlendMode(SDL_BLENDMODE_BLEND);
  aim_texture[frame_position]->SetAlphaMod(255);

  //std::cout << "angle: " << angle << std::endl;
  //std::cout << "Normalized angle: " << normalized_angle << std::endl;

  renderer.Copy(*aim_texture[frame_position], SDL2pp::NullOpt,
                SDL2pp::Rect((int)pos_x, pos_y, 21, 28), 0.0, SDL2pp::NullOpt,
                flip);
}

SDL_RendererFlip Worm::choose_flip_direction() {
  SDL_RendererFlip flip = SDL_FLIP_NONE; // Sin volteo por defecto
  if (direction == LEFT) {    // worm.get_direction() == LEFT
    // flip = SDL_FLIP_NONE;
  } else if (direction == RIGHT) { // worm.get_direction() == RIGHT
    flip = SDL_FLIP_HORIZONTAL;
  }
  return flip;
}
