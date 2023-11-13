#include <iostream>
#include "server_server.h"
#include "game_config.h"

#define SUCCESS 0
#define ERROR 1

int main(int argc, char* argv[]) { try {
	if (argc != 2) {
		std::cerr << "Bad program call. Expected " << argv[0] << " <servname>\n";
		return ERROR;
	}
	const std::string& servname = argv[1];

	// std::string config_path = "/game_config.yml";
	// GameConfig game_config(RESOURCES_PATH + config_path);
	// std::cout << "worm speed: " << game_config.get_worm_speed() << std::endl;
	// std::cout << "worm life: " << game_config.get_worm_life() << std::endl;

	Server server(servname);
	server.run();

	return SUCCESS;
} catch (const std::exception& err) {
	std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
	return ERROR;
 } catch (...) {
	 std::cerr << "Something went wrong and an unknown exception was caught.\n";
	 return ERROR;
}
}
