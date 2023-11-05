#include "server_protocol.h"

#include <cstdint>

#include "common_protocol_codes.h"

ServerProtocol::ServerProtocol(Socket& peer): peer(peer) {}

std::vector<char> ServerProtocol::receive_command(bool& was_closed) {
    unsigned int sz = 1;
    std::vector<char> buf(20);
    int s;
    std::vector<char> command;

    s = peer.recvall(buf.data(), sz, &was_closed);
    if (was_closed || s == 0) {
        return command;
    }

    command.push_back(RESEND_MSG_CODE);

    sz = 2;
    s = peer.recvall(buf.data(), sz, &was_closed);
    if (was_closed || s == 0) {
        command.clear();
        return command;
    }

    command.push_back(buf[0]);
    command.push_back(buf[1]);

    uint16_t msg_length = (uint16_t)(buf[0]) | (uint16_t)(buf[1]); // cambiar por ntoh

    buf.resize(msg_length);
    sz = msg_length;

    s = peer.recvall(buf.data(), sz, &was_closed);
    if (was_closed || s == 0) {
        command.clear();
        return command;
    }

    for (int i = 0; i < msg_length; i++) {
        command.push_back(buf[i]);
    }
    return command;
}

int ServerProtocol::send(std::vector<char> command, bool& was_closed) {
    int s = peer.sendall(command.data(), command.size(), &was_closed);
    if (was_closed || s == 0) {
        return 0;
    }

    return s;
}
