#ifndef INSERT_QUERY_RESULT_H
#define INSERT_QUERY_RESULT_H

#include <string>
#include <vector>

#include "../engine/DataType.h"
#include "../engine/TableField.h"

#include "QueryResult.h"

using namespace std;

class InsertQueryResult: public QueryResult {
    protected:
        int rowsNumber;
    public:
        InsertQueryResult(string table, int time, int rowsNumber);

        string toString();
};

#endif