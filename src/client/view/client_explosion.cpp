#include "client_explosion.h"

#include <cmath>
#include <box2d/b2_common.h>

#define EXPLOSION_DURATION_MS 3 * 1000

Explosion::Explosion(int pos_x, int pos_y, int radius, std::vector<float> ray_fractions, int start_frame,
                     SDL2pp::Renderer &rend) :
    pos_x(pos_x), pos_y(pos_y), radius(radius), ray_fractions(ray_fractions), frame_where_started(start_frame),
    renderer(rend) {}

void Explosion::update(int frame) {
  int delta_frames = frame - frame_where_started;
  if (delta_frames >= EXPLOSION_DURATION_MS) {
    finished = true;
  }
}

void Explosion::render(int frame, int camera_x, int camera_y) {
  SDL2pp::Color c(0, 0, 255);
  renderer.SetDrawColor(c);
  if (std::getenv("DEBUG") != NULL) {
    for (int i = 0; i < NUM_RAYS; i++) {
      float angle_rad = (i / (float)NUM_RAYS) * 2.0f * b2_pi;
      //std::cout << "fraction n° " << i << ": " << ray_fractions[i] << std::endl;
      float radius_fraction = ray_fractions[i] * radius;
      SDL2pp::Point center(pos_x - camera_x, pos_y - camera_y);
      SDL2pp::Point ray_dir(radius_fraction * sinf(angle_rad), -radius_fraction * cosf(angle_rad));
      SDL2pp::Point ray_end = (center + ray_dir);
      renderer.DrawLine(center, ray_end);
    }
    //std::cout << std::endl;
  }
}

bool Explosion::has_finished() { return finished; }