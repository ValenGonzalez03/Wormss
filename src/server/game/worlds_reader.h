#ifndef WORLDS_READER_H_
#define WORLDS_READER_H_

#include "../world/server_world.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>

class WorldsReader {
 private:
  std::map<std::string, std::filesystem::path> world_paths;

  // void load_file_info(const YAML::Node file, std::vector<std::shared_ptr<World>>& worlds) {
  //   std::shared_ptr<World> world = std::make_shared<World>();
  //   std::string name = file["name"].as<std::string>();
  //   std::string background = file["background"].as<std::string>();

  //   world->set_name(name);
  //   world->set_background(background);

  //   // Leo las vigas
  //   if (file["beams"]) {
  //     for (const auto& beam : file["beams"]) {
  //       float position_x = beam["position_x"].as<float>();
  //       float position_y = beam["position_y"].as<float>();
  //       float angle = beam["angle"].as<float>();
  //       float length = beam["length"].as<float>();
  //       // Agregar la viga al mundo (world)
  //       //std::cout << "Loading beam with angle " << angle << std::endl;
  //       world->create_beam(position_x, position_y, angle, length);
  //     }
  //   }
  //   // Leo los puntos de spawn
  //   if (file["spawn_points"]) {
  //     for (const auto& spawn_point : file["spawn_points"]) {
  //         float position_x = spawn_point["position_x"].as<float>();
  //         float position_y = spawn_point["position_y"].as<float>();
  //         // Agrego los puntos de spawn al vector de puntos de spawn
  //         world->add_spawn_point(position_x, position_y);
  //     }
  //   }
  //   else {
  //     std::cerr << "Error al leer el archivo: " << std::endl;
  //   }
  //   worlds.push_back(world);
  // }

  World read_world(std::filesystem::path& world_path);

 public:
  explicit WorldsReader();

  World generate_world(std::string world_name);

  std::vector<std::string> get_world_names();

  std::vector<std::shared_ptr<World>> read_yaml_files(const std::filesystem::path& directory) {
    std::vector<std::shared_ptr<World>> worlds;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
      try {
        YAML::Node file = YAML::LoadFile(entry.path().string());
        //load_file_info(file, worlds);
      } catch (const std::exception& e) {
        std::cerr << "Error al cargar el archivo YAML - " << e.what() << std::endl;
      }
    }
    return worlds;
  }
};

#endif