#ifndef TABLE_FIELD_H
#define TABLE_FIELD_H

#include <string>

#include "DataTypeEnum.h"

using namespace std;

class TableField {
    private:
        string name;
        DataTypeEnum type;
    public:
        static const int MAX_FIELD_LENGTH = 20;

        TableField(string name, DataTypeEnum type);

        string getName();
        DataTypeEnum getType();

        virtual string toString();

        virtual bool operator ==(const TableField &tableField) const;
};

#endif