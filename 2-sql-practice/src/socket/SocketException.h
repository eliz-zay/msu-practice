#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H

#include <exception>

using namespace std;

class SocketException: public std::exception {
protected:
    string message;
public:
    SocketException(string msg);
    const char * what() const throw ();
    
    ~SocketException() throw();
};

#endif
