#ifndef SELECT_OBJECT_H
#define SELECT_OBJECT_H

#include "QueryObject.h"
#include "condition_tree/OrCondition.h"

using namespace std;

class SelectObject: public QueryObject {
    protected:
        vector<string> fields;
        OrCondition* conditionTree;
    public:
        SelectObject(string table, vector<string> fields, OrCondition* conditionTree);
    
        vector<string> getFields();
        OrCondition* getConditionTree();

        string toString();
};

#endif
