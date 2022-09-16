#pragma once

#include "VarcharField.h"

VarcharField::VarcharField(string name, int length): TableField(name, DataTypeEnum::VARCHAR) {
    this->length = length;
}

int VarcharField::getLength() {
    return length;
}