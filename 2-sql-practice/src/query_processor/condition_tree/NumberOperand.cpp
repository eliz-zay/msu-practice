#pragma once

#include "NumberOperand.h"

NumberOperand::NumberOperand(long double value): BaseOperand(OperandTypeEnum::NUMBER) {
    this->value = value;
}

long double NumberOperand::getValue() {
    return this->value;
}
