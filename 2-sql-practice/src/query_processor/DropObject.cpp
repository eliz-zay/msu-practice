#pragma once

#include "DropObject.h"

DropObject::DropObject(string table): QueryObject(QueryTypeEnum::Drop, table) {
    
}

string DropObject::toString() {
    string dropString;
    
    dropString += "DropObject {\n";
    dropString += "   table: " + getTable() + "\n";
    dropString += "}\n";
    
    return dropString;
}
