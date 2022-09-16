#pragma once

#include "StringSetOperand.h"

#include "../../engine/Varchar.cpp"
#include "../../engine/EngineException.cpp"

StringSetOperand::StringSetOperand(set<string> value) : BaseOperand(OperandTypeEnum::STRING_SET) {
    this->value = value;
}

bool StringSetOperand::contains(Varchar inpString) {
    for (set<string>::iterator it = value.begin(); it != value.end(); ++it) {
        if (inpString == *it) {
            return true;
        }
    }
    return false;
}
