#include "client_worm_view.h"

WormView::WormView(SDL2pp::Renderer &rend,
                    std::vector<SDL2pp::Texture *> &walking)
      : renderer(rend), walking_texture(walking) {}

void WormView::render(int frame, Worm &worm, client_state &worm_state) {
  PositionConverter converter;
  Position pos_in_px = converter.convert_position_to_px(worm.get_position());
  int pos_x = pos_in_px.get_position_x();
  int pos_y = pos_in_px.get_position_y();
  /*
  std::cout << "Posx en m: " << worm.get_position().get_position_x()
            << std::endl;
  std::cout << "Posy en m: " << worm.get_position().get_position_y()
            << std::endl;
  std::cout << "Posx en px: " << pos_x << std::endl;
  std::cout << "Posy en px: " << pos_y << std::endl;
  */
  if (worm.get_state() == WORM_STATES::MOVING)
    render_worm_running(frame, pos_x, pos_y, worm, worm_state);
  else // worm.get_state() == idle == 0
    render_worm_idle(frame, pos_x, pos_y, worm, worm_state);
}

void WormView::render_worm_idle(int frame, int pos_x, int pos_y, Worm &worm,
                                client_state &worm_state) {
  worm_state.run_phase = 0;
  SDL_RendererFlip flip = choose_flip_direction(worm);
  walking_texture.front()->SetAlphaMod(255);
  walking_texture.front()->SetBlendMode(SDL_BLENDMODE_BLEND);
  renderer.Copy(*walking_texture.front(), SDL2pp::NullOpt, // Size
                SDL2pp::Rect(pos_x, pos_y, 60, 60),        // Destination
                0.0,                                       // don't rotate
                SDL2pp::NullOpt, // rotation center - not needed
                flip             // horizontal flip
  );
}

void WormView::render_worm_running(int frame, int pos_x, int pos_y, Worm &worm,
                                   client_state &worm_state) {
  SDL_RendererFlip flip = choose_flip_direction(worm);
  auto normalized_frame = frame / 40;
  walking_texture[normalized_frame % walking_texture.size()]->SetBlendMode(
      SDL_BLENDMODE_BLEND);

  walking_texture[normalized_frame % walking_texture.size()]->SetAlphaMod(
      255); // sprite is fully opaque
  renderer.Copy(*walking_texture[normalized_frame % walking_texture.size()],
                SDL2pp::NullOpt, SDL2pp::Rect((int)pos_x, pos_y, 60, 60), 0.0,
                SDL2pp::NullOpt, flip);
}

void WormView::render_worm_jumping(int frame, int pos_x, int pos_y, Worm &worm,
                                   client_state &worm_state) {}

SDL_RendererFlip WormView::choose_flip_direction(Worm &worm) {
  SDL_RendererFlip flip = SDL_FLIP_NONE; // Sin volteo por defecto
  if (worm.get_direction() == LEFT) {    // worm.get_direction() == LEFT
    // flip = SDL_FLIP_NONE;
  } else if (worm.get_direction() == RIGHT) { // worm.get_direction() == RIGHT
    flip = SDL_FLIP_HORIZONTAL;
  }
  return flip;
}