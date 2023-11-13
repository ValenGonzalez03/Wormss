#include "server_protocol.h"

#include <cstdint>

#include "../common/protocol_codes.h"
#include "commands/move.h"

ServerProtocol::ServerProtocol(Socket& peer): peer(peer) {}
// VER ESTO
/*
std::unique_ptr<Command> ServerProtocol::receive_command(bool& was_closed) {
    unsigned int sz = 1;
    std::vector<char> buf(1);
    int s;

    s = peer.recvall(buf.data(), sz, &was_closed);
    if (was_closed || s == 0) {
        //return;
    }

    char instruction = buf[0];

    if (instruction == MOVE) {
        int id = 1; //id de prueba
        return std::make_unique<Move>(id);
    }

    return nullptr;
}
*/
int ServerProtocol::send(GameState& game_state, bool& was_closed) {
    std::vector<char> command;
    int s = peer.sendall(command.data(), command.size(), &was_closed);
    if (was_closed || s == 0) {
        return 0;
    }

    return s;
}
