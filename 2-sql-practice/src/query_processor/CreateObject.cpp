#pragma once

#include "CreateObject.h"

CreateObject::CreateObject(string table, vector<TableField*> tableFields): QueryObject(QueryTypeEnum::Create, table) {
    this->tableFields = tableFields;
}

vector<TableField*> CreateObject::getTableFields() {
    return this->tableFields;
}

string CreateObject::toString() {
    string createString;
    
    createString += "CreateObject {\n";
    createString += "   table: " + getTable() + ",\n";
    createString += "   tableFields: [\n";
    
    for (int i = 0; i < tableFields.size(); ++i) {
        createString += "      " + tableFields[i]->toString();
        
        if (i != tableFields.size() - 1) {
            createString += ",";
        }
        
        createString += "\n";
    }
    
    createString += "   ]\n";
    createString += "}\n";
    
    return createString;
}
