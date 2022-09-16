#ifndef STRING_OPERAND_H
#define STRING_OPERAND_H

#include <string>

#include "BaseOperand.h"

class StringOperand: public BaseOperand {
    protected:
        string value;
    public:
        StringOperand(string value);
    
        string getValue();
};

#endif
