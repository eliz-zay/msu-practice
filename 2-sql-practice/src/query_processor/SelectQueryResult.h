#ifndef SELECT_QUERY_RESULT_H
#define SELECT_QUERY_RESULT_H

#include <string>
#include <vector>

#include "../engine/DataType.h"
#include "../engine/TableField.h"

#include "QueryResult.h"

using namespace std;

class SelectQueryResult: public QueryResult {
    protected:
        vector<TableField*> fields;
        vector<vector<DataType*> > rows;
    public:
        SelectQueryResult(string table, int time, vector<TableField*> fields, vector<vector<DataType*> > rows);

        string toString();
};

#endif