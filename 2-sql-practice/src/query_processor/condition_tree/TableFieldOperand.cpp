#pragma once

#include "TableFieldOperand.h"

TableFieldOperand::TableFieldOperand(string value) : BaseOperand(OperandTypeEnum::TABLE_FIELD) {
    this->value = value;
}

string TableFieldOperand::getValue() {
    return value;
}