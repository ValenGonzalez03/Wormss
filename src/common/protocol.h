#ifndef COMMON_PROTOCOL_H_
#define COMMON_PROTOCOL_H_

#include <utility>
#include "socket.h"
#include <cstdint>

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
};

#endif
