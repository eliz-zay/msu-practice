#ifndef BASE_CONDITION_H
#define BASE_CONDITION_H

#include <vector>

#include "../../engine/TableField.h"
#include "../../engine/DataType.h"

using namespace std;

class BaseCondition {
    public:
        virtual bool calculate(vector<TableField*> fields, vector<DataType*> row) = 0;
        virtual string toString(int nestLevel = 1) = 0;
};

#endif
