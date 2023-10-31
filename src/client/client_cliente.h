#ifndef CLIENT_CLIENTE_H
#define CLIENT_CLIENTE_H

#include <string>

class Cliente {
private:
    
public:

    explicit Cliente(const std::string& hostname, const std::string& servname);

    void run();

};

#endif
