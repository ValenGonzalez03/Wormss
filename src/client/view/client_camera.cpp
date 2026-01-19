#include "client_camera.h"
#include "../../common/game_constants.h"
#include "../../common/position.h"

#define SCROLL_SPEED 8

Camera::Camera(int width, int height): camera_box(SDL2pp::Rect(0, 0, width, height)) {}

void Camera::update() {
  if (moving_up) {
      if (camera_box.GetY() > 0)
        camera_box.y -= SCROLL_SPEED;
    }
    if (moving_down) {
      if (camera_box.GetY() < convert_meters_to_pixels_x(WORLD_HEIGHT) - camera_box.GetH())
        camera_box.y += SCROLL_SPEED;
    }
    if (moving_left) {
      if (camera_box.GetX() > 0)
        camera_box.x -= SCROLL_SPEED;
    }
    if (moving_right) {
      if (camera_box.GetX() < convert_meters_to_pixels_x(WORLD_WIDTH) - camera_box.GetW())
        camera_box.x += SCROLL_SPEED;
    }
}

int Camera::get_x() { return camera_box.GetX(); }
int Camera::get_y() { return camera_box.GetY(); }

void Camera::start_moving_up() { moving_up = true; }
void Camera::start_moving_down() { moving_down = true; }
void Camera::start_moving_left() { moving_left = true; }
void Camera::start_moving_right() { moving_right = true; }

void Camera::stop_moving_up() { moving_up = false; }
void Camera::stop_moving_down() { moving_down = false; }
void Camera::stop_moving_left() { moving_left = false; }
void Camera::stop_moving_right() { moving_right = false; }