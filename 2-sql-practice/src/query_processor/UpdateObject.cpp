#pragma once

#include "UpdateObject.h"

UpdateObject::UpdateObject(string table, TableField* field, DataType* value, BaseCondition* conditionTree)
    : QueryObject(QueryTypeEnum::Update, table) {
        this->table = table;
        this->field = field;
        this->value = value;
        this->conditionTree = conditionTree;
}

TableField* UpdateObject::getField() {
    return field;
}

DataType* UpdateObject::getValue() {
    return value;
}

BaseCondition* UpdateObject::getConditionTree() {
    return conditionTree;
}

string UpdateObject::toString() {
    string updateString;

    updateString += "UpdateObject {\n";
    updateString += "   table: " + getTable() + ",\n";
    updateString += "   field: " + field->toString() + ",\n";
    updateString += "   value: " + value->toString();
    if (conditionTree) {
        updateString += ",\n";
        updateString += "   conditionTree:\n" + conditionTree->toString(3);
    } else {
        updateString += "\n";
    }
    updateString += "}\n";

    return updateString;
}
