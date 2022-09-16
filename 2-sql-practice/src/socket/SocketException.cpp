#pragma once

#include "SocketException.h"

#include <iostream>
#include <exception>

SocketException::SocketException(string msg):message(msg) {
}

const char * SocketException::what() const throw () {
    return message.c_str();
}


SocketException::~SocketException() throw() {
}
