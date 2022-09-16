#ifndef VARCHAR_FIELD_H
#define VARCHAR_FIELD_H

#include "TableField.h"

using namespace std;

class VarcharField: public TableField {
    static const int MAX_LENGTH = 255;
    
    private:
        int length;
    public:
        VarcharField(string name, int length);

        int getLength();
};

#endif