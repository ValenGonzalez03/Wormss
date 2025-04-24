#ifndef CLIENT_POSITION_H_
#define CLIENT_POSITION_H_

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

static const float PIXEL_PER_METER = 23.33f; // 23,3 periodico
static float WORLD_HEIGHT = 360.0f;

static int convert_meters_to_pixels_x(float pos_x) {
  return pos_x * PIXEL_PER_METER;
}

static int convert_meters_to_pixels_y(float pos_y) {
  return WORLD_HEIGHT - pos_y * PIXEL_PER_METER;
}

static float convert_pixels_to_meters_x(int pos_x) {
  return pos_x / PIXEL_PER_METER;
}

static float convert_pixels_to_meters_y(int pos_y) {
  return (WORLD_HEIGHT - pos_y) / PIXEL_PER_METER;
}

#endif
