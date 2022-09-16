#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include "BaseSocket.h"
#include "ClientSocket.h"

class ServerSocket : public BaseSocket {
    private:
        static const int MAX_LISTENERS_NUMBER = 5;
    public:
        ServerSocket(int port);

        void listen();
        ClientSocket accept();
};

#endif