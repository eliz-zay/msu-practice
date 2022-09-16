#ifndef BASE_SOCKET_H
#define BASE_SOCKET_H

#include "SocketException.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>

using namespace std;

class BaseSocket {
    private:
        static const int BUFFER_SIZE = 1E5;
    protected:
        int socketFd;
        string host;
        int port;
        bool isClosed = false;

        BaseSocket();
    public:
        string read();
        void write(string message);
        void close();

        ~BaseSocket();
};

#endif
