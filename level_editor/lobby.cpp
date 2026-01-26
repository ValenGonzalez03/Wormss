#include "lobby.h"

void Lobby::run() {
  char choice;
  do {
    show_menu();
    std::cin >> choice;

    switch (choice) {
      case 'c':
        create_new_level();
        break;
      case 'e':
        edit_existing_level();
        break;
      case 'q':
        std::cout << "Saliendo del lobby.\n";
        break;
      default:
        std::cout << "Opción no válida. Inténtalo de nuevo.\n";
    }
  } while (choice != 'q');
}


void Lobby::show_menu() const {
  std::cout << "----- Lobby -----\n";
  std::cout << "Seleccione una opción:\n";
  std::cout << "c - Crear un nuevo nivel\n";
  std::cout << "e - Editar un nivel existente\n";
  std::cout << "q - Salir\n";
}


void Lobby::create_new_level() {
  FileEditor file_editor;
  World new_world;

  std::string file_path = std::string(RESOURCES_PATH) + "/Worlds/";
  std::string level_name = get_lvl_name_from_usr();
  std::string file_name = file_path + level_name + ".yml";

  new_world.set_name(level_name);
  new_world.set_background_name("Background1.jpg");

  LevelEditor level_editor(new_world);
  level_editor.run();


  file_editor.create_yaml_file(file_name, new_world);
}


std::string Lobby::get_lvl_name_from_usr() {
  std::string levelName;

  while (true) {
    std::cout << "Ingrese el nombre del nivel: ";
    std::cin >> levelName;

    std::string filePath = std::string(RESOURCES_PATH) + "/Worlds/" + levelName + ".yml";
    if (std::filesystem::exists(filePath)) {
      std::cout << "Por favor, elija otro nombre.\n";
    } else {
      // No existe un archivo con ese nombre, se puede utilizar
      break;
    }
  }
  return levelName;
}


void Lobby::edit_existing_level() {
  FileEditor file_editor;
  std::cout << "Seleccione un nivel para editar:\n";

  // Ruta de la carpeta que contiene los archivos de niveles
  std::string folderPath = std::string(RESOURCES_PATH) + "/Worlds/";

  int count = 1;
  std::vector<std::string> levelFiles;

  // Recorro la carpeta y muestro los archivos de niveles disponibles
  for (const auto& entry : fs::directory_iterator(folderPath)) {
    std::cout << count << " - " << entry.path().filename() << '\n';
    levelFiles.push_back(entry.path().string());
    count++;
  }

  int selectedLevel;
  std::cout << "Seleccione el número del nivel que desea editar: ";
  std::cin >> selectedLevel;

  if (selectedLevel > 0 && selectedLevel <= static_cast<int>(levelFiles.size())) {
    std::cout << "Abriendo y editando el nivel: " << levelFiles[selectedLevel - 1] << '\n';
    // Leo el archivo y lo cargo en un World
    World existing_world = file_editor.read_existing_level(levelFiles[selectedLevel - 1]);
    // Edito el World
    LevelEditor level_editor(existing_world);
    level_editor.run();

    //Lo guardo con el mismo nombre
    file_editor.create_yaml_file(levelFiles[selectedLevel - 1], existing_world);
  } else {
    std::cout << "Selección no válida. Volviendo al lobby.\n";
  }
}