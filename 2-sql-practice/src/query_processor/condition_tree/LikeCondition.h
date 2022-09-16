#ifndef LIKE_CONDITION_H
#define LIKE_CONDITION_H

#include "BinaryCondition.h"

using namespace std;

class LikeCondition : public BinaryCondition, public NegatableCondition {
    public:
        LikeCondition(BaseOperand* operand1, BaseOperand* operand2, bool doNegate = false);

        bool calculate(vector<TableField*> fields, vector<DataType*> row);
        string toString(int nestLevel = 1);
};

#endif
