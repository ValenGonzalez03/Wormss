#ifndef CLIENT_WORLDVIEW_H_
#define CLIENT_WORLDVIEW_H_

#include "client_beam.h"
#include "client_resource_pool.h"
#include <vector>

class WorldView {
private:
  ResourcePool &resource_pool;
  SDL2pp::Renderer &renderer;
  std::vector<Beam> beams;

public:
  WorldView(ResourcePool &res_pool, SDL2pp::Renderer &rend)
      : resource_pool(res_pool), renderer(rend) {}

  void add_short_beam(int pos_x, int pos_y);

  void add_long_beam(int pos_x, int pos_y);

  // void update(GameState &game_state);

  void render(int frame);
};

#endif