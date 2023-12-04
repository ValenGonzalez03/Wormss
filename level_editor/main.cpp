#include <iostream>
#include "lobby.h"

#define ERROR 1
#define SUCCESS 0

int main(int argc, char *argv[]) {
  try {
    if (argc != 1) {
      std::cerr << "Bad program call. Expected " << argv[0];
      return ERROR;
    }

    Lobby lobby;
    lobby.run();

    return SUCCESS;

  } catch (const std::exception &err) {
    std::cerr << "Something went wrong and an exception was caught: "
              << err.what() << "\n";
    return ERROR;
  } catch (...) {
    std::cerr << "Something went wrong and an unknown exception was caught.\n";
    return ERROR;
  }
}
