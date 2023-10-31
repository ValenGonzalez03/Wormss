#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <string>
#include <utility>
#include "../common/socket.h"
#include "../common/protocol.h"
#include "client_sender_thread.h"
#include "client_receiver_thread.h"

class Client{
    private:
        Protocol prot;
        ClientSenderThread sender;
        ClientReceiverThread receiver;
    public:
        Client(Socket&& skt);

        void start_threads();

        void join_threads();

};

#endif
