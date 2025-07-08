#ifndef GAME_CONSTANTS_H_
#define GAME_CONSTANTS_H_

#define PIXEL_PER_METER 23.33f // 23,3 periodico (1 metro equivale a 23,33 pixeles)
#define WORLD_HEIGHT 360 // En pixeles

#define WORM_WIDTH 0.8154f // En metros (Equivale aproximadamente a unos 19 pixeles)
#define WORM_HEIGHT 1.0729f // En metros (Equivale aproximadamente a unos 25 pixeles)

#define BEAM_HEIGHT 0.8583f // En metros (Equivale aproximadamente a unos 20 pixeles)

#define MISSILE_WIDTH 1.0f // En metros (Equivale aproximadamente a unos 5 pixeles)
#define MISSILE_HEIGHT 0.8f // En metros (Equivale aproximadamente a unos 5 pixeles)

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#define JUMP_FORWARD 0
#define JUMP_BACKWARD 1

enum WormState { IDLE, MOVING, JUMPING, AIMING, SHOOTING };

struct BeamData {
    float pos_x;
    float pos_y;
    int angle;
    float width;
};

#endif