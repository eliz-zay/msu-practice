#pragma once

#include "InsertObject.h"

InsertObject::InsertObject(string table): QueryObject(QueryTypeEnum::Insert, table) {

}

void InsertObject::setFieldValues(vector<DataType*> fieldValues) {
    this->fieldValues = fieldValues;
}

vector<DataType*> InsertObject::getFieldValues() {
    return this->fieldValues;
}

string InsertObject::toString() {
    string insertString;
    
    insertString += "InsertObject {\n";
    insertString += "   table: " + getTable() + ",\n";
    insertString += "   fieldValues: [\n";
    
    for (int i = 0; i < fieldValues.size(); ++i) {
        insertString += "      " + fieldValues[i]->toString();
        
        if (i != fieldValues.size() - 1) {
            insertString += ",";
        }
        
        insertString += "\n";
    }
    
    insertString += "   ]\n";
    insertString += "}\n";
    
    return insertString;
}
