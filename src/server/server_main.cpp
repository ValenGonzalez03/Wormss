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
