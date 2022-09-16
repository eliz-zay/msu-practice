#ifndef MULTIPLE_CONDITION_H
#define MULTIPLE_CONDITION_H

#include <vector>

#include "BaseCondition.h"

#include "../../engine/TableField.h"
#include "../../engine/DataType.h"

using namespace std;

class MultipleCondition: public BaseCondition {
    protected:
        vector<BaseCondition*> operands;
    public:
        void addOperand(BaseCondition* operand);
    
        virtual bool calculate(vector<TableField*> fields, vector<DataType*> row) = 0;
        virtual string toString(int nestLevel = 1) = 0;
};

#endif
