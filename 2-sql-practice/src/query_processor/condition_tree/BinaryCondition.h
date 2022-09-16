#ifndef BINARY_CONDITION_H
#define BINARY_CONDITION_H

#include "BaseCondition.h"

#include "BaseOperand.h"

using namespace std;

class BinaryCondition : public BaseCondition {
    protected:
        BaseOperand* operand1;
        BaseOperand* operand2;
    public:
        BinaryCondition(BaseOperand* operand1, BaseOperand* operand2);

        virtual bool calculate(vector<TableField*> fields, vector<DataType*> row) = 0;
        virtual string toString(int nestLevel = 1) = 0;
};

#endif
