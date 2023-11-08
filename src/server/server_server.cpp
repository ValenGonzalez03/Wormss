#include "server_server.h"
#include "../common/socket.h"
#include <iostream>
#include "../common/protocol.h"

Server::Server(const std::string& servname): servname(servname) {}

void Server::run() {
	Socket accepter_skt(servname.c_str());

    //Aceptador accept_thread(skt);
    //accept_thread.start();

    Socket peer(accepter_skt.accept());
    Protocol prot(std::move(peer));

    while (true) {
        std::unique_ptr<Command> cmd = prot.process_command();
        cmd->run();
    }
    
    while (std::cin.get() != 'q') {}
    /*
    accept_thread.kill();
    accept_thread.join();
	*/
    return;
}
