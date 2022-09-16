#ifndef NUMBER_SET_OPERAND_H
#define NUMBER_SET_OPERAND_H

#include <set>

#include "BaseOperand.h"
#include "../../engine/Number.h"
#include "../../engine/EngineException.h"

using namespace std;

class NumberSetOperand : public BaseOperand {
    protected:
        set<long double> value;
    public:
        NumberSetOperand(set<long double> value);

        bool contains(Number number);
};

#endif
