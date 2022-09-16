#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <vector>
#include <iostream>
#include <string>

#include "socket/ServerSocket.cpp"
#include "socket/ClientSocket.cpp"

#include "query_parser/QueryParser.cpp"
#include "query_parser/QueryParserException.cpp"

#include "query_processor/QueryObject.cpp"
#include "query_processor/SelectObject.cpp"
#include "query_processor/QueryProcessor.cpp"

#include "ServerConfig.h"

using namespace std;

void handler(ClientSocket clientSocket) {
    string message;
    
    while (1) {
        cout << "TipaSQL => ";
        getline(cin, message);
        cout << endl;
        clientSocket.write(message);
        message = clientSocket.read();
        cout << message << endl;
    }
}

int main() {
    try {
        ClientSocket clientSocket(ServerConfig::HOST, ServerConfig::PORT);
        clientSocket.handle(handler, false);
    } catch (const SocketException &ex) {
        cout << "Socket exception: " << ex.what() << endl;
        exit(0);
    }

    return 0;
}
