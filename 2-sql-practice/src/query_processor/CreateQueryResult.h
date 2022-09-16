#ifndef CREATE_QUERY_RESULT_H
#define CREATE_QUERY_RESULT_H

#include <string>
#include <vector>

#include "../engine/DataType.h"
#include "../engine/TableField.h"

#include "QueryResult.h"

using namespace std;

class CreateQueryResult: public QueryResult {
    public:
        CreateQueryResult(string table, int time);

        string toString();
};

#endif