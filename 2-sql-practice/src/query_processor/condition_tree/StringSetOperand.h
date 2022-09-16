#ifndef STRING_SET_OPERAND_H
#define STRING_SET_OPERAND_H

#include <set>
#include <string>

#include "BaseOperand.h"
#include "../../engine/Varchar.h"
#include "../../engine/EngineException.h"

using namespace std;

class StringSetOperand : public BaseOperand {
    protected:
        set<string> value;
    public:
        StringSetOperand(set<string> value);

        bool contains(Varchar inpString);
};

#endif
