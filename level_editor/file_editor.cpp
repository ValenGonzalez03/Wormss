#include "file_editor.h"

void FileEditor::create_yaml_file(const std::string& file_path, World& world) {
  std::string level_name = world.get_name();
  std::string background_name = world.get_background_name();

  YAML::Emitter emitter;
  
  emitter << YAML::BeginMap;
  emitter << YAML::Key << "name" << YAML::Value << "Nivel 2";
  emitter << YAML::Key << "background" << YAML::Value << "background2.jpg";

  emitter << YAML::Key << "beams" << YAML::Value << YAML::BeginSeq;
  for (auto& beam : world.get_beams()) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "position_x" << YAML::Value << beam.get_pos_x();
    emitter << YAML::Key << "position_y" << YAML::Value << beam.get_pos_y();
    emitter << YAML::Key << "angle" << YAML::Value << beam.get_angle();
    emitter << YAML::Key << "length" << YAML::Value << beam.get_length();
    emitter << YAML::EndMap;
  }
  emitter << YAML::EndSeq;

  emitter << YAML::Key << "spawn_points" << YAML::Value << YAML::BeginSeq;
  for (auto &spawn_point : world.get_spawn_points()) {
    emitter << YAML::BeginMap;
    emitter << YAML::Key << "position_x" << YAML::Value << spawn_point.get_pos_x();
    emitter << YAML::Key << "position_y" << YAML::Value << spawn_point.get_pos_y();
    emitter << YAML::EndMap;
  }
  emitter << YAML::EndSeq;

  emitter << YAML::EndMap;

  // Guardo el YAML en un archivo
  std::ofstream file_out(file_path);
  file_out << emitter.c_str();
  file_out.close();
}


World FileEditor::read_existing_level(const std::string& file_path) {
    YAML::Node file = YAML::LoadFile(file_path);
    World world;
    std::string name = file["name"].as<std::string>();
    std::string background = file["background"].as<std::string>();

    world.set_name(name);
    world.set_background_name(background);

    // Leo las vigas
    if (file["beams"]) {
      for (const auto& beam : file["beams"]) {      
        float position_x = beam["position_x"].as<float>();
        float position_y = beam["position_y"].as<float>();
        float angle = beam["angle"].as<float>();
        float length = beam["length"].as<float>();
        Beam new_beam(position_x, position_y, angle, length);
        // Agregar la viga al mundo (world)
        world.add_beam(new_beam);
      }
    }

    // Leo los puntos de spawn
    if (file["spawn_points"]) {
      for (const auto& spawn_point : file["spawn_points"]) {
        float position_x = spawn_point["position_x"].as<float>();
        float position_y = spawn_point["position_y"].as<float>();
        // Agrego los puntos de spawn al vector de puntos de spawn
        SpawnPoint spawn(position_x, position_y);
        world.add_spawn_point(spawn);
      }
    }

    else {
      throw std::runtime_error("Error durante la lectura del archivo");
    }
    return world;
  }