#include "LikeCondition.h"

#include "TableFieldOperand.cpp"
#include "StringOperand.cpp"

#include "../../shared/VectorHelper.cpp"
#include "../../shared/StringHelper.cpp"

#include "../../engine/Varchar.cpp"

#include <string>

LikeCondition::LikeCondition(BaseOperand* operand1, BaseOperand* operand2, bool isNegate) : BinaryCondition(operand1, operand2), NegatableCondition(isNegate) {
    
}

bool LikeCondition::calculate(vector<TableField*> fields, vector<DataType*> row) {
    if (
        operand1->getType() != OperandTypeEnum::TABLE_FIELD ||
        operand2->getType() != OperandTypeEnum::STRING
    ) {
        cout << "Wrong arguments" << endl;
        // TODO: throw exception
    }
    
    TableFieldOperand* fieldOperand = dynamic_cast<TableFieldOperand*>(operand1);
    TableField* field = new TableField(fieldOperand->getValue(), DataTypeEnum::VARCHAR);

    int fieldIndex = VectorHelper::findInPointerVector(fields, field);
    if (fieldIndex == -1) {
        cout << "Couldn't find field with this type";
        // TODO: throw exception
    }

    Varchar* varcharOperand = dynamic_cast<Varchar*>(row[fieldIndex]);
    StringOperand* stringOperand = dynamic_cast<StringOperand*>(operand2);

    string regex = stringOperand->getValue();
    regex = regex.substr(1, regex.size() - 2);

    StringHelper::replace(regex, "%", ".*");
    StringHelper::replace(regex, "_", ".");

    return StringHelper::matches(varcharOperand->getValue(), regex)^isNegated;
}

string LikeCondition::toString(int nestLevel) {
    string message;
    
    message += string(3*(nestLevel - 1),' ');
    
    if (this->NegatableCondition::isNegated)
        message += "NOT ";
        
    message += "Like Condition\n";
    
    return message;
}
