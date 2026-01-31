#include "server_server.h"
#include "comm/server_accept_thread.h"
#include <iostream>
#include "../common/lib/socket.h"

Server::Server(const std::string &servname) : servname(servname) {}

void Server::run() {
  Socket skt(servname.c_str());

  Accept accept_thread(skt);
  accept_thread.start();

  while (std::cin.get() != 'q') {
    sleep(1);
  }

  accept_thread.kill();
  accept_thread.join();

  return;
}
