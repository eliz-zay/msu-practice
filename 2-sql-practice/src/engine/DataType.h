#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <string>
#include <iostream>

#include "DataTypeEnum.h"

using namespace std;

class DataType {
    protected:
        DataTypeEnum type;
    public:
        DataType(DataTypeEnum type);

        DataTypeEnum getType();

        virtual string toString() = 0;
};

#endif