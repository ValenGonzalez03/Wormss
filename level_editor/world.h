#ifndef WORLD_H_
#define WORLD_H_

#include "beam.h"
#include "spawn_point.h"
#include <string>
#include <vector>

class World {
private:
    std::string name = "";
    std::string background_name = "";
    std::vector<Beam> beams;
    std::vector<SpawnPoint> spawn_points;

public:
    World() {}

    void set_name(std::string new_name) {
        this->name = new_name;
    }

    void set_background_name (std::string new_backgorund_name) {
        this->background_name = new_backgorund_name;
    }

    std::string get_name() {
        return this->name;
    }

    std::string get_background_name() {
        return this->background_name;
    }

    std::vector<Beam> get_beams() {
        return this->beams;
    }

    std::vector<SpawnPoint> get_spawn_points() {
        return this->spawn_points;
    }

    void add_beam(Beam beam) {
        this->beams.push_back(beam);
    }

    void add_spawn_point(SpawnPoint spawn) {
        this->spawn_points.push_back(spawn);
    }


};

#endif