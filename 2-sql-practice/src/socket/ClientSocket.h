#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include "BaseSocket.h"

class ClientSocket : public BaseSocket {
    public:
        ClientSocket(string host, int port);
        ClientSocket(int socketFd, string host, int port);
        void handle(void (*handler)(ClientSocket clientSocket), bool shouldCreateProcess = true);
};

#endif