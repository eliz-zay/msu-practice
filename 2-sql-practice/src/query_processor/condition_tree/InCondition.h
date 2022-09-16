#ifndef IN_CONDITION_H
#define IN_CONDITION_H

#include "BinaryCondition.h"

using namespace std;

class InCondition : public BinaryCondition, public NegatableCondition {
    public:
        InCondition(BaseOperand* operand1, BaseOperand* operand2, bool doNegate = false);

        bool calculate(vector<TableField*> fields, vector<DataType*> row);
        string toString(int nestLevel = 1);
};

#endif
