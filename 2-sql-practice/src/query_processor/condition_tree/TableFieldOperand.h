#ifndef TABLE_FIELD_OPERAND_H
#define TABLE_FIELD_OPERAND_H

#include <set>
#include <string>

#include "BaseOperand.h"

using namespace std;

class TableFieldOperand : public BaseOperand {
    protected:
        string value;
    public:
        TableFieldOperand(string value);
        string getValue();
};

#endif
