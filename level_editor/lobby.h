#ifndef LOBBY_H_
#define LOBBY_H_

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include "level_editor.h"
#include "file_editor.h"

namespace fs = std::filesystem;

class Lobby {
 public:

  Lobby() {}

  void run();

 private:
  void show_menu() const;

  void create_new_level();

  std::string get_lvl_name_from_usr();

  void edit_existing_level();
};

#endif
