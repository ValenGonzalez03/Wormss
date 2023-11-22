#ifndef CLIENT_WORM_VIEW_H_
#define CLIENT_WORM_VIEW_H_

#include "client_resource_pool.h"
#include "../common/game_state.h"
#include "client_position_converter.h"
#include "client_state.h"
#include "../common/worm_states.h"

class WormView {
private:
  SDL2pp::Renderer &renderer;
  //SDL2pp::Texture &standing_texture;
  SDL2pp::Texture &walking_texture;
  //SDL2pp::Texture &shooting_texture;
public:
  // Crea un WormView con un renderer y las texturas correspondientes
  WormView(SDL2pp::Renderer &rend, SDL2pp::Texture &walking) :
  renderer(rend), walking_texture(walking) {}

  // Renderiza el worm pasado por parámetro según el estado del
  // mismo
  void render(int frame, Worm &worm, client_state &worm_state);

  // Renderiza el worm cuando este se encuentra en estado "idle"
  void render_worm_idle(int frame, int pos_x, int pos_y, Worm &worm, client_state &worm_state);

  // Renderiza el worm cuando este se encuentra en estado "running"
  void render_worm_running(int frame, int pos_x, int pos_y, Worm &worm, client_state &worm_state);

  SDL_RendererFlip choose_flip_direction(Worm &worm) {
    SDL_RendererFlip flip = SDL_FLIP_NONE; // Sin volteo por defecto
    if (worm.get_direction() == LEFT) { //worm.get_direction() == LEFT
      //flip = SDL_FLIP_NONE;
    }
    else if (worm.get_direction() == RIGHT) { //worm.get_direction() == RIGHT
      flip = SDL_FLIP_HORIZONTAL;
    }
    return flip;
  }

};

#endif