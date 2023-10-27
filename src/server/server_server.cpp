#include "server_server.h"
#include <iostream>

Server::Server(const std::string& servname): servname(servname) {}

void Server::run() {
	//Socket skt(servname.c_str());

    //Aceptador accept_thread(skt);
    //accept_thread.start();
    
    while (std::cin.get() != 'q') {}
    /*
    accept_thread.kill();
    accept_thread.join();
	*/
    return;
}
