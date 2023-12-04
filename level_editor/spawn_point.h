#ifndef SPAWN_POINT_H_
#define SPAWN_POINT_H_

class SpawnPoint{
private:
    float pos_x;
    float pos_y;
public:
    SpawnPoint(float posx, float posy) : pos_x(posx), pos_y(posy) {}

    float get_pos_x() {
        return this->pos_x;
    }

    float get_pos_y() {
        return this->pos_y;
    }
};

#endif