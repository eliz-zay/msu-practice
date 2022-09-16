#pragma once

#include "DataTypeEnum.h"

#include "EngineStatusEnum.h"
#include "EngineException.cpp"

#include "Number.h"

using namespace std;

Number::Number(long double value): DataType(DataTypeEnum::NUMBER) {
    this->value = value;
}

Number* Number::parse(string valueString) {
    try {
        long double value = stold(valueString);
        return new Number(value);
    } catch(const exception& e) {
        throw EngineException(EngineStatusEnum::InvalidValue);
    }
}

Number* Number::create(long double value) {
    return new Number(value);
}

long double Number::getValue() {
    return value;
}

string Number::toString() {
    return to_string(this->value);
}

bool Number::operator == (const Number &number) const {
    return this->value == number.value;
}
bool Number::operator == (const long double numberValue) const {
    return this->value == numberValue;
}

bool Number::operator > (const Number &number) const {
    return this->value > number.value;
}
bool Number::operator > (const long double numberValue) const {
    return this->value > numberValue;
}

bool Number::operator < (const Number &number) const {
    return this->value < number.value;
}
bool Number::operator < (const long double numberValue) const {
    return this->value < numberValue;
}

bool Number::operator >= (const Number &number) const {
    return this->value >= number.value;
}
bool Number::operator >= (const long double numberValue) const {
    return this->value >= numberValue;
}

bool Number::operator <= (const Number &number) const {
    return this->value <= number.value;
}
bool Number::operator <= (const long double numberValue) const {
    return this->value <= numberValue;
}

bool Number::operator != (const Number &number) const {
    return this->value != number.value;
}
bool Number::operator != (const long double numberValue) const {
    return this->value != numberValue;
}
