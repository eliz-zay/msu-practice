#ifndef DELETE_QUERY_RESULT_H
#define DELETE_QUERY_RESULT_H

#include <string>
#include <vector>

#include "../engine/DataType.h"
#include "../engine/TableField.h"

#include "QueryResult.h"

using namespace std;

class DeleteQueryResult: public QueryResult {
    protected:
        int rowsNumber;
    public:
        DeleteQueryResult(string table, int time, int rowsNumber);

        string toString();
};

#endif