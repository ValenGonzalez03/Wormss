#ifndef GAME_CONFIG_H_
#define GAME_CONFIG_H_

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <string>
#include <vector>

class GameConfig {
 private:
  float worm_speed;
  int worm_life;

  void load_info(const YAML::Node file) {
    worm_speed = file["worm_speed"].as<float>();
    worm_life = file["worm_life"].as<int>();
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

  int get_worm_life() const { return worm_life; }
};

#endif
