#pragma once

#include "TableRow.h"

#include "DataTypeHelper.cpp"

TableRow::TableRow() {
    deleted = false;
}

TableRow::TableRow(vector<DataType*> values) {
    this->values = values;
    deleted = false;
}

void TableRow::addValue(DataType* value) {
    values.push_back(value);
}

void TableRow::setValue(int index, DataType* value) {
    values[index] = value;
}

bool TableRow::isDeleted() {
    return deleted;
}

void TableRow::setDeleted() {
    deleted = true;
}

vector<DataType*> TableRow::getValues() {
    return values;
}

string TableRow::toString() {
    string rowString;

    rowString += "TableRow { values: [ ";
    for (int i = 0; i < values.size(); i++) {
        rowString += values[i]->toString();
        if (i != values.size() - 1) {
            rowString += ", ";
        }
    }
    rowString += " ], deleted: ";
    rowString += deleted ? "true" : "false";
    rowString += " }";

    return rowString;
}

bool TableRow::operator == (const TableRow &tableRow) const {
    if (this->values.size() != tableRow.values.size()) {
        return false;
    }

    for (int i = 0; i < this->values.size(); i++) {
        if (!DataTypeHelper::compareEqual(this->values[i], tableRow.values[i])) {
            return false;
        }
    }

    return true;
}
