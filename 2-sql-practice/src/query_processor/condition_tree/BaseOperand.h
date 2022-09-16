#ifndef BASE_OPERAND_H
#define BASE_OPERAND_H

#include "OperandTypeEnum.h"

class BaseOperand {
    protected:
        OperandTypeEnum type;
    public:
        BaseOperand();
        BaseOperand(OperandTypeEnum type);

        virtual OperandTypeEnum getType();
};

#endif
