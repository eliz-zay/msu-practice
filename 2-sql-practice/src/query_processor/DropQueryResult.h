#ifndef DROP_QUERY_RESULT_H
#define DROP_QUERY_RESULT_H

#include <string>
#include <vector>

#include "../engine/DataType.h"
#include "../engine/TableField.h"

#include "QueryResult.h"

using namespace std;

class DropQueryResult: public QueryResult {
    public:
        DropQueryResult(string table, int time);

        string toString();
};

#endif