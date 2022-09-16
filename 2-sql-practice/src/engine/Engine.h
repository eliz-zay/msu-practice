#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

#include "Table.h"
#include "TableField.h"
#include "TableRow.h"

#include "EngineException.h"

using namespace std;

namespace Engine {
    void createTable(Table table) throw(EngineException);
    Table loadTable(string tableName, bool withRows = false) throw(EngineException);
    void insertIntoTable(string tableName, vector<TableRow> rows) throw(EngineException);
    void updateValuesInTable(string tableName, vector<TableRow> rows, TableField* field, DataType* value) throw(EngineException);
    void deleteFromTable(string table, vector<TableRow> rows) throw(EngineException);
    void deleteTable(string table) throw(EngineException);
}

#endif