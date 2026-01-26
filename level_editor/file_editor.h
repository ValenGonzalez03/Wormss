#include "yaml-cpp/yaml.h"
#include <fstream>
#include <iostream>
#include "beam.h"
#include "spawn_point.h"
#include "world.h"

class FileEditor {
 public:

  // Crea un archivo yaml a partir del mundo pasado por parámetro
  void create_yaml_file(const std::string& file_path, World& world);

  // Lee un archivo yaml y devuelve un World con los datos
  World read_existing_level(const std::string& file_path);
};