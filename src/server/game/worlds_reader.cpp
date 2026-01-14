#include "worlds_reader.h"
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>

WorldsReader::WorldsReader() {
  std::filesystem::path root_directory = std::filesystem::path(RESOURCES_PATH) / "Worlds";
  for (const auto& entry : std::filesystem::directory_iterator(root_directory)) {
    try {
      auto file_path = entry.path();
      YAML::Node file = YAML::LoadFile(file_path.string());
      std::string world_name = file["name"].as<std::string>();
      
      world_paths[world_name] = file_path;
    } catch (const std::exception& e) {
      std::cerr << "Error al leer el archivo YAML - " << e.what() << std::endl;
    }
  }
}

World WorldsReader::read_world(std::filesystem::path& world_path) {
  World world;

  YAML::Node file = YAML::LoadFile(world_path.string());

  
  std::string background = file["background"].as<std::string>();
  world.set_background(background);

  
  // Leo las vigas
  if (file["beams"]) {
    for (const auto& beam : file["beams"]) {      
      float position_x = beam["position_x"].as<float>();
      float position_y = beam["position_y"].as<float>();
      float angle = beam["angle"].as<float>();
      float length = beam["length"].as<float>();
      
      float angle_radians = static_cast<float>(angle) * b2_pi / 180.0f;
      world.create_beam(position_x, position_y, angle_radians, length);
    }
  }
  // Leo los puntos de spawn
  if (file["spawn_points"]) {
    for (const auto& spawn_point : file["spawn_points"]) {
        float position_x = spawn_point["position_x"].as<float>();
        float position_y = spawn_point["position_y"].as<float>();
        
        world.add_spawn_point(position_x, position_y);
    }
  }
  return world;
}

World WorldsReader::generate_world(std::string world_name) {
  World world;
  auto world_path = world_paths[world_name];
  try
  {
    world = read_world(world_path);
  }
  catch(const std::exception& e)
  {
    std::cerr << "Ha ocurrido un error generando el mundo: " << e.what() << '\n';
  }
  
  return world;
}

std::vector<std::string> WorldsReader::get_world_names() {
  std::vector<std::string> world_names;
  for (auto pair : world_paths) {
    world_names.push_back(pair.first);
  }
  return world_names;
}
