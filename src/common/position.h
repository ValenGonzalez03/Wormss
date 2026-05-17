#ifndef CLIENT_POSITION_H_
#define CLIENT_POSITION_H_

#include "game_constants.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

// Recibe un numero en metros y devuelve el equivalente en pixeles
static int convert_meters_to_pixels_x(float pos_x) { return pos_x * PIXEL_PER_METER; }

// Recibe un numero en metros y devuelve el equivalente en pixeles (Exclusivamente para convertir posiciones en el eje Y)
static int convert_meters_to_pixels_y(float pos_y) {
  return convert_meters_to_pixels_x(WORLD_HEIGHT) - pos_y * PIXEL_PER_METER;
}

static float convert_pixels_to_meters_x(int pos_x) { return pos_x / PIXEL_PER_METER; }

static float convert_pixels_to_meters_y(int pos_y) {
  return (WORLD_HEIGHT - pos_y) / PIXEL_PER_METER;
}

#endif
