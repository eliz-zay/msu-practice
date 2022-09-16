#pragma once

#include "QueryParserException.h"
#include "QueryStatusStrings.h"

#include <iostream>
#include <exception>

QueryParserException::QueryParserException(QueryStatusEnum status): status(status) {
}

const char * QueryParserException::what() const throw () {
    string message = "Query Exception: " + queryStatusStrings[static_cast<int>(this->status)] + "\n";
    return message.c_str();
}

QueryParserException::~QueryParserException() throw() {
}
