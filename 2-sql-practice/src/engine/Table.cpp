#pragma once

#include "Table.h"

Table::Table() {}

Table::Table(string name, vector<TableField*> fields) {
    this->name = name;
    this->fields = fields;
}

Table::Table(string name, vector<TableField*> fields, vector<TableRow> rows) {
    this->name = name;
    this->fields = fields;
    this->rows = rows;
}

vector<TableField*> Table::getFields() {
    return this->fields;
}

vector<TableRow> Table::getRows() {
    return this->rows;
}

string Table::getName() {
    return name;
}

string Table::toString() {
    string tableString;
    tableString += "Table {\n";
    tableString += "  name: " + this->name + ",\n";

    tableString += "  fields: [\n";
    for (TableField* field : this->fields) {
        tableString += "    " + field->toString() + "\n";
    }
    tableString += "  ]\n";

    tableString += "  rows: [\n";
    for (TableRow row: this->rows) {
        tableString += "    " + row.toString() + "\n";
    }
    tableString += "  ]\n";

    tableString += "}";

    return tableString;
}

ostream& operator<< (ostream &out, const Table &table) {
    out << "Table {" << endl;
    out << "  name: " << table.name << "," << endl;

    out << "  fields: [" << endl;
    for (TableField* field : table.fields) {
        out << "    " << field->toString() << endl;
    }
    out << "  ]" << endl;

    out << "  rows: [" << endl;
    for (TableRow row: table.rows) {
        cout << "    " <<row.toString() << endl;
    }
    out << "  ]" << endl;

    out << "}";

    return out;
}