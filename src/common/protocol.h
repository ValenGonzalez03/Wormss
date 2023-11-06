#ifndef COMMON_PROTOCOL_H_
#define COMMON_PROTOCOL_H_

#include <utility>
#include <memory>
#include <cstdint>
#include <iostream>

#include "socket.h"
#include "command.h"
#include "start_moving.h"
#include "stop_moving.h"

class Protocol {
private:
    Socket skt;
public:
    explicit Protocol(Socket&& socket);

    // Elimino posibilidad de copias y operador =
    Protocol(const Protocol&) = delete;
    Protocol& operator=(const Protocol&) = delete;

    // Implemento move semantics
    Protocol(Protocol&&) = default;
    Protocol& operator=(Protocol&&) = default;

    void send_command(Command &cmd);

    std::unique_ptr<Command> process_command();
};

#endif
