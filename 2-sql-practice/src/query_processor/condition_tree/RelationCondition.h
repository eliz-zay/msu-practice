#ifndef RELATION_CONDITION_H
#define RELATION_CONDITION_H

#include "BinaryCondition.h"
#include "RelationTypeEnum.h"

using namespace std;

class RelationCondition : public BinaryCondition {
    protected:
        RelationTypeEnum relationType;
    public:
        RelationCondition(BaseOperand* operand1, BaseOperand* operand2, string relationType);

        bool calculate(vector<TableField*> fields, vector<DataType*> row);
        string toString(int nestLevel = 1);
};

#endif
