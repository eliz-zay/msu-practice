#pragma once

#include "SelectObject.h"

SelectObject::SelectObject(string table, vector<string> fields, OrCondition* conditionTree): QueryObject(QueryTypeEnum::Select, table) {
    this->fields = fields;
    this->conditionTree = conditionTree;
}

vector<string> SelectObject::getFields() {
    return this->fields;
}

OrCondition* SelectObject::getConditionTree() {
    return this->conditionTree;
}

string SelectObject::toString() {
    string selectString;
    
    selectString += "SelectObject {\n";
    selectString += "   table: " + getTable() + "\n";
    selectString += "   fields: ";
    for (int i = 0; i < fields.size(); i++) {
        if (i != fields.size() - 1) {
            selectString += fields[i] + ", ";
        } else {
            selectString += fields[i];
        }
    }
    if (conditionTree) {
        selectString += ",\n";
        selectString += "   conditionTree:\n" + conditionTree->toString(3);
    } else {
        selectString += "\n";
    }
    selectString += "}\n";
    
    return selectString;
}
