#ifndef CLIENT_WORLDVIEW_H_
#define CLIENT_WORLDVIEW_H_

#include <vector>
#include "client_beam.h"

class WorldView{
private:
    ResourcePool resource_pool;
    std::vector<Beam> beams;

public:
    WorldView();

    void add_beam(Position beam_pos);
};

#endif