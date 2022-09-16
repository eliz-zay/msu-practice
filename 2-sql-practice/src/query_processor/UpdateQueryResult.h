#ifndef UPDATE_QUERY_RESULT_H
#define UPDATE_QUERY_RESULT_H

#include <string>
#include <vector>

#include "../engine/DataType.h"
#include "../engine/TableField.h"

#include "QueryResult.h"

using namespace std;

class UpdateQueryResult: public QueryResult {
    protected:
        int rowsNumber;
    public:
        UpdateQueryResult(string table, int time, int rowsNumber);

        string toString();
};

#endif