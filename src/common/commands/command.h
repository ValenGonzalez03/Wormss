#ifndef COMMAND_H_
#define COMMAND_H_

#include "../lib/queue.h"
#include "../lib/socket.h"
#include <cstdint>

class Command {
 protected:
  uint8_t code;
  uint8_t client_id;

 public:
  Command(uint8_t cod, uint8_t clt_id) : code(cod), client_id(clt_id) {}

  // Debe enviar el codigo y los datos adicionales
  // de ser necesario
  virtual void send(Socket &skt, bool *was_closed) const = 0;  // NOLINT(runtime/references)

  // El código lo recibe el protocolo para decidir
  // el comando a crear. Este receive solo recibe los datos
  // adicionales del comando en caso de ser necesario.
  virtual void receive(Socket &skt, bool *was_closed) = 0;  // NOLINT(runtime/references)

  // PROVISORIAS
  virtual uint8_t get_client_id() = 0;
  virtual uint8_t get_game_id() = 0;
};

#endif
