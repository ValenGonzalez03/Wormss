#ifndef GAME_CONFIG_H_
#define GAME_CONFIG_H_

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>
#include <vector>

class GameConfig {
 private:
  float worm_speed;
  uint32_t worm_health;

  void load_info(const YAML::Node file) {
    worm_speed = file["worm_speed"].as<float>();
    worm_health = file["worm_health"].as<uint32_t>();
  }

 public:
  explicit GameConfig(const std::string& filename) {
    try {
      YAML::Node file = YAML::LoadFile(filename);
      load_info(file);
    } catch (const YAML::Exception& e) {
      std::cerr << "Error al leer el archivo YAML: " << e.what() << std::endl;
    }
  }

  float get_worm_speed() const { return worm_speed; }

  uint32_t get_worm_health() const { return worm_health; }
};

#endif
