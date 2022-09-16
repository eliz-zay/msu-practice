#pragma once

#include "BaseOperand.h"


BaseOperand::BaseOperand() {

}

BaseOperand::BaseOperand(OperandTypeEnum type) {
    this->type = type;
}

OperandTypeEnum BaseOperand::getType() {
    return type;
}
