#ifndef QUERY_OBJECT_H
#define QUERY_OBJECT_H

#include <map>
#include <string>
#include <vector>

#include "QueryTypeEnum.h"

using namespace std;

class QueryObject {
    protected:
        QueryTypeEnum type;
        string table;
    public:
        QueryObject(QueryTypeEnum type, string table);
        QueryTypeEnum getType();
        string getTable();

        virtual string toString() = 0;
};

#endif
