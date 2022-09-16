#ifndef NUMBER_OPERAND_H
#define NUMBER_OPERAND_H

#include "BaseOperand.h"
#include "../../engine/Number.h"
#include "../../engine/EngineException.h"

class NumberOperand: public BaseOperand {
    protected:
        long double value;
    public:
        NumberOperand(long double value);
    
        long double getValue();
};

#endif
