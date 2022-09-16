#ifndef OR_CONDITION_H
#define OR_CONDITION_H

#include <vector>

#include "MultipleCondition.h"
#include "NegatableCondition.h"

#include "../../engine/TableField.h"
#include "../../engine/DataType.h"

using namespace std;

class OrCondition: public MultipleCondition, public NegatableCondition {
    public:
        bool calculate(vector<TableField*> fields, vector<DataType*> row);
        string toString(int nestLevel = 1);
};

#endif
