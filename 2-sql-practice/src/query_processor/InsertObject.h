#ifndef INSERT_OBJECT_H
#define INSERT_OBJECT_H

#include "QueryObject.h"
#include "../engine/DataType.h"

class InsertObject: public QueryObject {
    protected:
        vector<DataType*> fieldValues;
    public:
        InsertObject(string table);
    
        void setFieldValues(vector<DataType*> fieldValues);
        vector<DataType*> getFieldValues();

        string toString();
};

#endif
