#include "protocol.h"

Protocol::Protocol(Socket&& socket): skt(std::move(socket)) {}