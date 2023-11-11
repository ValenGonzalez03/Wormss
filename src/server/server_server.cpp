#include "server_server.h"
#include <iostream>
#include <list>

Server::Server(const std::string &servname) : servname(servname) {}

void Server::run() {
  Socket accepter_skt(servname.c_str());

  // Aceptador accept_thread(skt);
  // accept_thread.start();

  Socket peer(accepter_skt.accept());
  Protocol prot(std::move(peer));

  while (true) {
    std::unique_ptr<Command> cmd = prot.process_command();
    cmd->run();

    std::list list({Worm(Position(1, 3)), Worm(Position(2, 5))});
    GameState game_state(list);
    prot.send_game_state(game_state);
  }

  while (std::cin.get() != 'q') {
  }
  /*
  accept_thread.kill();
  accept_thread.join();
      */
  return;
}
