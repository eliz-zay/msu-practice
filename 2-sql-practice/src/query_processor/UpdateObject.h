#ifndef UPDATE_OBJECT_H
#define UPDATE_OBJECT_H

#include "QueryObject.h"
#include "condition_tree/BaseCondition.h"

#include "../engine/DataType.h"
#include "../engine/TableField.h"

class UpdateObject: public QueryObject {
    protected:
        TableField* field;
        DataType* value;
        BaseCondition* conditionTree;
    public:
        UpdateObject(string table, TableField* field, DataType* value, BaseCondition* conditionTree = nullptr);

        TableField* getField();
        DataType* getValue();
        BaseCondition* getConditionTree();

        string toString();
};

#endif
