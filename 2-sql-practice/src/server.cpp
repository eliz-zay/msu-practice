#include <iostream>
#include <string>

#include "socket/ServerSocket.cpp"
#include "socket/ClientSocket.cpp"
#include "socket/SocketException.cpp"

#include "query_parser/QueryParser.cpp"
#include "query_parser/QueryParserException.cpp"

#include "query_processor/QueryObject.cpp"
#include "query_processor/SelectObject.cpp"
#include "query_processor/QueryProcessor.cpp"

#include "engine/EngineException.cpp"

#include "ServerConfig.h"

using namespace std;

void handler(ClientSocket clientSocket) {
    try {
        while (1) {
            string message = clientSocket.read();
            string result;
            
            try {
                QueryObject* queryObject = QueryParser::parseQuery(message);
                result = QueryProcessor::executeQuery(queryObject)->toString();
            } catch(const QueryParserException &ex) {
                result = ex.what();
            } catch(const EngineException &ex) {
                result = ex.what();
            }

            clientSocket.write(result);
        }
    } catch (const SocketException &ex) {
        cout << "Socket exception: " << ex.what() << endl;
        exit(0);
    }
}

int main() {
    try {
       ServerSocket serverSocket(ServerConfig::PORT);
       serverSocket.listen();

       while (1) {
            ClientSocket clientSocket = serverSocket.accept();
            clientSocket.handle(handler);
       }

       serverSocket.close();
    } catch (const SocketException &ex) {
        cout << "Socket exception: " << ex.what() << endl;
    }
    
    return 0;
}
