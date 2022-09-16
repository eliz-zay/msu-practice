#ifndef QUERY_PARSER_EXCEPTION_H
#define QUERY_PARSER_EXCEPTION_H

#include "QueryStatusEnum.h"

#include <exception>
#include <string>

using namespace std;

class QueryParserException: public std::exception {
protected:
    QueryStatusEnum status;
public:
    QueryParserException(QueryStatusEnum status);
    const char * what() const throw ();
        
    ~QueryParserException() throw();
};

#endif
