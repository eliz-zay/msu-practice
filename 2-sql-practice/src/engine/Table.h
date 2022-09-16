#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <string>
#include <vector>

#include "TableField.h"
#include "TableRow.h"

using namespace std;

class Table {
    private:
        string name;
        vector<TableField*> fields;
        vector<TableRow> rows;
    public:
        Table();
        Table(string name, vector<TableField*> fields);
        Table(string name, vector<TableField*> fields, vector<TableRow> rows);

        string getName();
        vector<TableField*> getFields();
        vector<TableRow> getRows();

        string toString();

        friend ostream& operator<< (ostream &out, const Table &table);
};

#endif