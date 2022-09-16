#pragma once

#include "ServerSocket.h"

#include "BaseSocket.cpp"
#include "ClientSocket.cpp"

#include "SocketException.cpp"

ServerSocket::ServerSocket(int port) {
    struct sockaddr_in serverParams;

    serverParams.sin_family = AF_INET;
    serverParams.sin_addr.s_addr = INADDR_ANY;
    serverParams.sin_port = htons(port);

    this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socketFd < 0) {
        throw SocketException("Couldn't create socket");
    }

    int bindResult = bind(this->socketFd, (struct sockaddr *)&serverParams, sizeof(serverParams));
    if (bindResult < 0) {
        throw SocketException("Couldn't bind socket");
    }

    this->port = port;
    this->host = "127.0.0.1";

    cout << "Server started listening on port " << this->port << endl;
}

void ServerSocket::listen() {
    ::listen(this->socketFd, this->MAX_LISTENERS_NUMBER);
}

ClientSocket ServerSocket::accept() {
    socklen_t clientParamsLength;
    struct sockaddr_in clientParams;
    
    int clientFd = ::accept(this->socketFd, (struct sockaddr *)&clientParams, &clientParamsLength);

    return ClientSocket(clientFd, this->host, this->port);
}
