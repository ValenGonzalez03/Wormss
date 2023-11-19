#ifndef CLIENT_WORM_VIEW_H_
#define CLIENT_WORM_VIEW_H_

#include "client_resource_pool.h"
#include "../common/game_state.h"
#include "client_position_converter.h"
#include "client_state.h"

class WormView {
private:
  SDL2pp::Renderer &renderer;
  //SDL2pp::Texture &standing_texture;
  SDL2pp::Texture &walking_texture;
  //SDL2pp::Texture &shooting_texture;
public:
  WormView(SDL2pp::Renderer &rend, SDL2pp::Texture &walking) :
  renderer(rend), walking_texture(walking) {}

  void render(int frame, Worm &worm, client_state &worm_state);

  void render_worm_idle(int pos_x, int pos_y, Worm &worm, client_state &worm_state);

  void render_worm_running(int pos_x, int pos_y, Worm &worm, client_state &worm_state);

};

#endif