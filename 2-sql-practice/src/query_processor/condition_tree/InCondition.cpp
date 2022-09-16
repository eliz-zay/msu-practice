#pragma once

#include "InCondition.h"

#include "TableFieldOperand.cpp"
#include "NumberSetOperand.cpp"
#include "StringSetOperand.cpp"
#include "../../shared/VectorHelper.cpp"

#include <iostream>
#include <string>

using namespace std;

InCondition::InCondition(BaseOperand* operand1, BaseOperand* operand2, bool isNegate) : BinaryCondition(operand1, operand2), NegatableCondition(isNegate) {

}

bool InCondition::calculate(vector<TableField*> fields, vector<DataType*> row) {
    if (
        operand1->getType() != OperandTypeEnum::TABLE_FIELD ||
        !(operand2->getType() == OperandTypeEnum::NUMBER_SET || operand2->getType() == OperandTypeEnum::STRING_SET)
    ) {
        cout << "Wrong arguments" << endl;
        return false;
        // TODO: throw exception
    }

    TableFieldOperand* fieldOperand = dynamic_cast<TableFieldOperand*>(operand1);

    TableField* field;
    if (operand2->getType() == OperandTypeEnum::NUMBER_SET) {
        field = new TableField(fieldOperand->getValue(), DataTypeEnum::NUMBER);
    } else {
        field = new TableField(fieldOperand->getValue(), DataTypeEnum::VARCHAR);
    }

    int fieldIndex = VectorHelper::findInPointerVector(fields, field);
    if (fieldIndex == -1) {
        cout << "Couldn't find field with this type" << endl;
        return false;
        // TODO: throw exception
    }

    if (operand2->getType() == OperandTypeEnum::NUMBER_SET) {
        NumberSetOperand* setOperand = dynamic_cast<NumberSetOperand*>(operand2);
        Number* numberOperand = dynamic_cast<Number*>(row[fieldIndex]);
        return setOperand->contains(*numberOperand)^isNegated;
    } else {
        StringSetOperand* setOperand = dynamic_cast<StringSetOperand*>(operand2);
        Varchar* stringOperand = dynamic_cast<Varchar*>(row[fieldIndex]);
        return setOperand->contains(*stringOperand)^isNegated;
    }
}

string InCondition::toString(int nestLevel) {
    string message;
    
    message += string(3*(nestLevel - 1),' ');
    
    if (this->NegatableCondition::isNegated)
        message += "NOT ";
        
    message += "In Condition\n";
    
    return message;
}
