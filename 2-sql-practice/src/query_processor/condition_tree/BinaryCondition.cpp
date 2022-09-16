#pragma once

#include "BinaryCondition.h"

BinaryCondition::BinaryCondition(BaseOperand* operand1, BaseOperand* operand2) {
    this->operand1 = operand1;
    this->operand2 = operand2;
}