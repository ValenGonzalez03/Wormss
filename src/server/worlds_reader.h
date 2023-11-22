#ifndef SCENARIO_H_
#define SCENARIO_H_

#include "world.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <filesystem>
#include <fstream>

class WorldsReader{
private:

  void load_file_info(const YAML::Node file, std::vector<World>& worlds) {
    World world;
    std::string name = file["name"].as<std::string>();
    std::string background = file["background"].as<std::string>();

    //world.add_name(name);
    //world.add_background(background);

    // Leo las vigas
    if (file["beams"]) {
      for (const auto& beam : file["beams"]) {      
        float position_x = beam["position_x"].as<float>();
        float position_y = beam["position_y"].as<float>();
        float angle = beam["angle"].as<float>();
        float length = beam["length"].as<float>();
        // Agregar la viga al mundo (world)
        world.create_beam(position_x, position_y, angle, length);
      }
    }
    // Leo los puntos de spawn
    if (file["spawn_points"]) {
      for (const auto& spawn_point : file["spawn_points"]) {
          float position_x = spawn_point["position_x"].as<float>();
          float position_y = spawn_point["position_y"].as<float>();
          world.create_worm(position_x, position_y);
      }
    }
    else {
      std::cerr << "Error al leer el archivo: " << std::endl;
    }
    worlds.push_back(world);
  }

public:
  explicit WorldsReader();

  std::vector<World> read_yaml_files(const std::filesystem::path& directory) {
    std::vector<World> worlds;
    for (const auto& filename : std::filesystem::directory_iterator(directory)) {
      YAML::Node file = YAML::LoadFile(filename);
      load_file_info(file, worlds);
    }
    return worlds;
  }

/*
    WorldsReader(const std::string& filename) {
        try {
            YAML::Node file = YAML::LoadFile(filename);
            load_info(file);
        }
        catch (const YAML::Exception& e) {
            std::cerr << "Error al leer el archivo YAML: " << e.what() << std::endl;
        }
    }

    float get_worm_speed() {
        return worm_speed;
    }

    int get_worm_life() {
        return worm_life;
    }
*/
};



#endif