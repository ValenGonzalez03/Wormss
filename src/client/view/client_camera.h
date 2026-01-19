#ifndef CLIENT_CAMERA_H_
#define CLIENT_CAMERA_H_

#include <SDL2pp/SDL2pp.hh>
#include <SDL2/SDL.h>

class Camera {
private:
SDL2pp::Rect camera_box;

bool moving_up = false;
bool moving_down = false;
bool moving_left = false;
bool moving_right = false;

public:

    explicit Camera(int width, int height);

    void update();

    int get_x();
    int get_y();

    void start_moving_up();
    void start_moving_down();
    void start_moving_left();
    void start_moving_right();

    void stop_moving_up();
    void stop_moving_down();
    void stop_moving_left();
    void stop_moving_right();

};


#endif