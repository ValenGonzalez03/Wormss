#ifndef CLIENT_WORM_VIEW_H_
#define CLIENT_WORM_VIEW_H_

#include "client_resource_pool.h"
#include "../common/game_state.h"
#include "client_position_converter.h"

class WormView {
private:
    SDL2pp::Renderer &renderer;
    //SDL2pp::Texture &standing_texture;
    SDL2pp::Texture &walking_texture;
    //SDL2pp::Texture &shooting_texture;
public:
    WormView(SDL2pp::Renderer &rend, SDL2pp::Texture &walking) :
    renderer(rend), walking_texture(walking) {}

  void render(int frame, Worm &worm) {
    PositionConverter converter;
    int pos_x = converter.convert_from_m_to_px(worm.get_pos_x());
    int pos_y = converter.convert_from_m_to_px(worm.get_pos_y());
    walking_texture.SetAlphaMod(255);
    renderer.Copy(
      walking_texture, 
      SDL2pp::Rect(10, 10, 40, 40),          // Size
      SDL2pp::Rect(pos_x, pos_y, 40, 40),    // Destination
      0.0,                                   // don't rotate
      SDL2pp::NullOpt,                       // rotation center - not needed
      SDL_FLIP_HORIZONTAL                    // horizontal flip
  );
    /*
    texture.SetAlphaMod(255);
    int sbeam_height = texture.GetHeight();
    int sbeam_width = texture.GetWidth();
    renderer.Copy(texture,
                  SDL2pp::Rect(0, 0, sbeam_width - 10, sbeam_height), // Size
                  SDL2pp::Rect(position.get_position_x(),
                               position.get_position_y(), sbeam_width,
                               sbeam_height)); // Destination
    */
  }
};

#endif