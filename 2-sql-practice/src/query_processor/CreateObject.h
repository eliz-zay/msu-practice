#ifndef CREATE_OBJECT_H
#define CREATE_OBJECT_H

#include "QueryObject.h"

#include "../engine/TableField.h"

class CreateObject: public QueryObject {
    protected:
        vector<TableField*> tableFields;
    public:
        CreateObject(string table, vector<TableField*> tableFields);
    
        vector<TableField*> getTableFields();
    
        virtual string toString();
};

#endif
