#pragma once

#include <string>

#include "DataTypeEnum.h"

#include "EngineStatusEnum.h"
#include "EngineException.cpp"

#include "Varchar.h"

using namespace std;

Varchar::Varchar(string value): DataType(DataTypeEnum::VARCHAR) {
    this->value = value;
}

Varchar* Varchar::parse(string valueString) {
    if (valueString[0] != '\'' || valueString[valueString.size() - 1] != '\'') {
        throw EngineException(EngineStatusEnum::InvalidValue);
    }

    return new Varchar(valueString.substr(1, valueString.size() - 2));
}

Varchar* Varchar::create(string value) {
    return new Varchar(value);
}

string Varchar::toString() {
    return '\'' + this->value + '\'';
}

string Varchar::getValue() {
    return this->value;
}

bool Varchar::operator == (const Varchar &string) const {
    return this->value == string.value;
}
bool Varchar::operator == (const string stringValue) const {
    return this->value == stringValue;
}

bool Varchar::operator > (const Varchar &string) const {
    return this->value > string.value;
}
bool Varchar::operator > (const string stringValue) const {
    return this->value > stringValue;
}

bool Varchar::operator < (const Varchar &string) const {
    return this->value < string.value;
}
bool Varchar::operator < (const string stringValue) const {
    return this->value < stringValue;
}

bool Varchar::operator >= (const Varchar &string) const {
    return this->value >= string.value;
}
bool Varchar::operator >= (const string stringValue) const {
    return this->value >= stringValue;
}

bool Varchar::operator <= (const Varchar &string) const {
    return this->value <= string.value;
}
bool Varchar::operator <= (const string stringValue) const {
    return this->value <= stringValue;
}

bool Varchar::operator != (const Varchar &string) const {
    return this->value != string.value;
}
bool Varchar::operator != (const string stringValue) const {
    return this->value != stringValue;
}