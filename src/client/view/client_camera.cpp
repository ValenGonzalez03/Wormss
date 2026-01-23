#include "client_camera.h"
#include "../../common/game_constants.h"
#include "../../common/position.h"

#define SCROLL_SPEED 8
#define PORTION_SCROLLABLE 8

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

bool Camera::is_up_corner(int mouse_x, int mouse_y) {
  return mouse_y < camera_box.h / PORTION_SCROLLABLE;
}

bool Camera::is_down_corner(int mouse_x, int mouse_y) {
  return mouse_y > camera_box.h * (1 - 1.0f / float(PORTION_SCROLLABLE)) && mouse_y < camera_box.h;
}

bool Camera::is_left_corner(int mouse_x, int mouse_y) {
  return mouse_x < camera_box.w / PORTION_SCROLLABLE;
}

bool Camera::is_right_corner(int mouse_x, int mouse_y) {
  return mouse_x > camera_box.w * (1 - 1.0f / float(PORTION_SCROLLABLE)) && mouse_x < camera_box.w;
}

void Camera::check_mouse_position(int mouse_x, int mouse_y) {
  moving_up = is_up_corner(mouse_x, mouse_y);
  moving_down = is_down_corner(mouse_x, mouse_y);
  moving_left = is_left_corner(mouse_x, mouse_y);
  moving_right = is_right_corner(mouse_x, mouse_y);
}

void Camera::stop_scrolling() {
  moving_up = false;
  moving_down = false;
  moving_left = false;
  moving_right = false;
}

int Camera::get_x() { return camera_box.GetX(); }
int Camera::get_y() { return camera_box.GetY(); }
