#pragma once

#include <string>

#include "DataTypeEnum.h"
#include "DataType.cpp"
#include "Varchar.cpp"
#include "Number.cpp"

#include "EngineStatusEnum.h"
#include "EngineException.cpp"

using namespace std;

namespace DataTypeHelper {
    DataType* create(string valueString, DataTypeEnum dataTypeEnum) {
        if (dataTypeEnum == DataTypeEnum::NUMBER) {
            return Number::parse(valueString);
        } else if (dataTypeEnum == DataTypeEnum::VARCHAR) {
            return Varchar::parse(valueString);
        }
    }

    bool compareEqual(DataType* value1, DataType* value2) {
        if (value1->getType() != value2->getType()) {
            throw EngineException(EngineStatusEnum::WrongValueType);
        } else if (value1->getType() == DataTypeEnum::NUMBER) {
            Number number1 = *dynamic_cast<Number*>(value1);
            Number number2 = *dynamic_cast<Number*>(value2);
            return number1 == number2;
        } else if (value1->getType() == DataTypeEnum::VARCHAR) {
            Varchar varchar1 = *dynamic_cast<Varchar*>(value1);
            Varchar varchar2 = *dynamic_cast<Varchar*>(value2);
            return varchar1 == varchar2;
        }
    }

    bool compareLess(DataType* value1, DataType* value2) {
        if (value1->getType() != value2->getType()) {
            throw EngineException(EngineStatusEnum::WrongValueType);
        } else if (value1->getType() == DataTypeEnum::NUMBER) {
            Number number1 = *dynamic_cast<Number*>(value1);
            Number number2 = *dynamic_cast<Number*>(value2);
            return number1 < number2;
        } else if (value1->getType() == DataTypeEnum::VARCHAR) {
            Varchar varchar1 = *dynamic_cast<Varchar*>(value1);
            Varchar varchar2 = *dynamic_cast<Varchar*>(value2);
            return varchar1 < varchar2;
        }
    }

    bool compareGreater(DataType* value1, DataType* value2) {
        if (value1->getType() != value2->getType()) {
            throw EngineException(EngineStatusEnum::WrongValueType);
        } else if (value1->getType() == DataTypeEnum::NUMBER) {
            Number number1 = *dynamic_cast<Number*>(value1);
            Number number2 = *dynamic_cast<Number*>(value2);
            return number1 > number2;
        } else if (value1->getType() == DataTypeEnum::VARCHAR) {
            Varchar varchar1 = *dynamic_cast<Varchar*>(value1);
            Varchar varchar2 = *dynamic_cast<Varchar*>(value2);
            return varchar1 > varchar2;
        }
    }
}
