#pragma once

#include "SelectQueryResult.h"

#include "../engine/DataType.cpp"
#include "../engine/TableField.cpp"

#include "QueryResult.cpp"

#include <iomanip>
#include <sstream>

SelectQueryResult::SelectQueryResult(string table, int time, vector<TableField*> fields, vector<vector<DataType*> > rows): QueryResult(table, time) {
    this->fields = fields;
    this->rows = rows;
}

string SelectQueryResult::toString() {
    stringstream stream;

    stream << "SELECT" << endl;
    stream << string(17 * fields.size(), '-') << endl;
    
    for (int i = 0; i < fields.size(); i++) {
        stream << setw(15) << fields[i]->getName() << " |" << setw(15);
    }

    stream << endl;
    for (int i = 0; i < fields.size(); i++) {
        stream << string(16, '-') << '+';
    }
    // stream << string(17 * fields.size(), '-') << endl;
    stream << endl;

    for (vector<DataType*> row : rows) {
        for (int i = 0; i < row.size(); i++) {
            stream << setw(15) << row[i]->toString() << " |" << setw(15);
        }
        stream << endl;
    }

    stream << string(17 * fields.size(), '-') << endl;

    stream << "Execution took: " << time / 1000.f << " ms" << endl;

    return stream.str();
}