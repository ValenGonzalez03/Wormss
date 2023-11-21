#include "client_worm_view.h"

void WormView::render(int frame, Worm &worm, client_state &worm_state) {
  PositionConverter converter;
  Position pos_in_px = converter.convert_position_to_px(worm.get_position());
  int pos_x = pos_in_px.get_position_x();
  int pos_y = pos_in_px.get_position_y();
  std::cout << "Posx en m: " << worm.get_position().get_position_x() << std::endl;
  std::cout << "Posy en m: " << worm.get_position().get_position_y() << std::endl;
  std::cout << "Posx en px: " << pos_x << std::endl;
  std::cout << "Posy en px: " << pos_y << std::endl;
  if (worm.get_state() == WORM_STATES::MOVING)
      render_worm_running(frame, pos_x, pos_y, worm, worm_state);
  else // worm.get_state() == idle == 0
      render_worm_idle(frame, pos_x, pos_y, worm, worm_state);
}

void WormView::render_worm_idle(int frame, int pos_x, int pos_y, Worm &worm, client_state &worm_state) {
  worm_state.run_phase = 0;
  walking_texture.SetAlphaMod(255);
  walking_texture.SetBlendMode(SDL_BLENDMODE_BLEND);
  renderer.Copy(
    walking_texture, 
    SDL2pp::Rect(10, 10, 40, 40),          // Size
    SDL2pp::Rect(pos_x, pos_y, 40, 40),    // Destination
    0.0,                                   // don't rotate
    SDL2pp::NullOpt,                       // rotation center - not needed
    SDL_FLIP_HORIZONTAL                    // horizontal flip
);
}

void WormView::render_worm_running(int frame, int pos_x, int pos_y, Worm &worm, client_state &worm_state) {
  SDL_RendererFlip flip = SDL_FLIP_NONE; // Sin volteo por defecto
  walking_texture.SetBlendMode(SDL_BLENDMODE_BLEND);
  worm_state.run_phase = (frame / 50) % 15;
  int src_x = 10, src_y = 10; // by default, standing sprite
  // Voltear horizontalmente solo si te estás moviendo a la izquierda
  if (worm.get_direction() == LEFT) { //worm.get_direction() == LEFT
      //flip = SDL_FLIP_NONE;
  }
  else if (worm.get_direction() == RIGHT) { //worm.get_direction() == RIGHT
    flip = SDL_FLIP_HORIZONTAL;
  }
  src_x = 10;
  src_y = 10 + 60 * worm_state.run_phase; //Corregir?

  walking_texture.SetAlphaMod(255); // sprite is fully opaque
  renderer.Copy(
    walking_texture, 
    SDL2pp::Rect(src_x, src_y, 40, 40),       // Size
    SDL2pp::Rect((int)pos_x, pos_y, 40, 40),  // Destination
    0.0,                                      // don't rotate
    SDL2pp::NullOpt,                          // rotation center - not needed
    flip                                      // horizontal flip
  );
}