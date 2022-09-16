#pragma once

#include "DataType.h"

DataType::DataType(DataTypeEnum type) {
    this->type = type;
}

DataTypeEnum DataType::getType() {
    return type;
}