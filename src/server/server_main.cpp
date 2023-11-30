#include <iostream>
#include "server_server.h"
#include "worlds_reader.h"

#define SUCCESS 0
#define ERROR 1

int main(int argc, char* argv[]) { try {
	if (argc != 2) {
		std::cerr << "Bad program call. Expected " << argv[0] << " <servname>\n";
		return ERROR;
	}
	const std::string& servname = argv[1];

	std::string worlds_path = "/Worlds";
	WorldsReader worlds_reader;
	std::vector<std::shared_ptr<World>> worlds = worlds_reader.read_yaml_files(RESOURCES_PATH + worlds_path);
	
	for (auto &world : worlds) {
		std::cout << world.get()->get_name() << std::endl;
	}

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
