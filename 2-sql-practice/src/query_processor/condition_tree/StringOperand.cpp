#pragma once

#include "StringOperand.h"

StringOperand::StringOperand(string value): BaseOperand(OperandTypeEnum::STRING) {
    this->value = value;
}

string StringOperand::getValue() {
    return this->value;
}
