#include <iostream>
#include "client_client.h"

#define SUCCESS 0
#define ERROR 1

int main(int argc, char* argv[]) { try {
	if (argc != 3) {
		std::cerr << "Bad program call. Expected " << argv[0] << " <hostname> <servname>\n";
		return ERROR;
	}
    const std::string& hostname = argv[1];
	const std::string& servname = argv[2];

    Socket skt(hostname.c_str(), servname.c_str());
    Client client(std::move(skt));


    client.start_threads();
    client.join_threads();

	return SUCCESS;

    } catch (const std::exception& err) {
	    std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
	    return ERROR;
    } catch (...) {
	    std::cerr << "Something went wrong and an unknown exception was caught.\n";
	    return ERROR;
    }
}
