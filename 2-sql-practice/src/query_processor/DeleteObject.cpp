#pragma once

#include "DeleteObject.h"

DeleteObject::DeleteObject(string table, OrCondition* conditionTree): QueryObject(QueryTypeEnum::Delete, table) {
    this->conditionTree = conditionTree;
}

OrCondition* DeleteObject::getConditionTree() {
    return this->conditionTree;
}

string DeleteObject::toString() {
    string deleteString;
    
    deleteString += "DeleteObject {\n";
    deleteString += "   table: " + getTable();
    if (conditionTree) {
        deleteString += ",\n";
        deleteString += "   conditionTree:\n" + conditionTree->toString(3);
    } else {
        deleteString += "\n";
    }
    deleteString += "}\n";
    
    return deleteString;
}
