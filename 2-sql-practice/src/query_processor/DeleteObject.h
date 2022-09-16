#ifndef DELETE_OBJECT_H
#define DELETE_OBJECT_H

#include "QueryObject.h"
#include "condition_tree/OrCondition.h"

class DeleteObject: public QueryObject {
    protected:
        OrCondition* conditionTree;
    public:
        DeleteObject(string table, OrCondition* conditionTree = nullptr);
    
        OrCondition* getConditionTree();
    
        string toString();
};

#endif
