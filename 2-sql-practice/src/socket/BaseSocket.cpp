#pragma once

#include "BaseSocket.h"

BaseSocket::BaseSocket() {
    
}

string BaseSocket::read() {
    char buffer[this->BUFFER_SIZE];
    int bytesReceived = recv(this->socketFd, buffer, this->BUFFER_SIZE, 0);

    if (bytesReceived <= 0) {
        throw SocketException("Connection is broken");
    } else {
        return buffer;
    }
}

void BaseSocket::write(string message) {
    message += '\0';
    int bytesSent = send(this->socketFd, message.c_str(), message.length(), 0);

    if (bytesSent < 0) {
        throw SocketException("Connection is broken");
    }
}

void BaseSocket::close() {
    if (!this->isClosed) {
        ::close(this->socketFd);
        
        this->isClosed = true;
    }
}

BaseSocket::~BaseSocket() {
    if (!this->isClosed) {
        this->close();
    }
}
