#include "client_explosion.h"

#include <cmath>
#include <box2d/b2_common.h>

#define EXPLOSION_DURATION_MS 3 * 1000

Explosion::Explosion(int pos_x, int pos_y, int radius, int start_frame, SDL2pp::Renderer &rend) 
  :
  pos_x(pos_x), pos_y(pos_y), radius(radius), frame_where_started(start_frame), renderer(rend)
  {}

void Explosion::update(int frame) {
   int delta_frames = frame - frame_where_started;
   if (delta_frames >= EXPLOSION_DURATION_MS) {
      finished = true;
   }
}

void Explosion::render(int frame) {
  SDL2pp::Color c(0, 0, 255);
  renderer.SetDrawColor(c);
  if (std::getenv("DEBUG") != NULL) {
    for (int i = 0; i < NUM_RAYS; i++) {
      float angle_rad = (i / (float)NUM_RAYS) * 360 * (b2_pi / 180.0f);
      SDL2pp::Point center (pos_x, pos_y);
      SDL2pp::Point ray_dir ( radius * sinf(angle_rad), radius * cosf(angle_rad) );
      SDL2pp::Point ray_end = center + ray_dir;
      renderer.DrawLine(center, ray_end);
    }
  }
}

bool Explosion::has_finished() {
    return finished;
}