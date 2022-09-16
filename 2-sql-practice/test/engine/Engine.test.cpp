#include <string>
#include <vector>

#include "../../src/engine/DataType.cpp"
#include "../../src/engine/Varchar.cpp"
#include "../../src/engine/Number.cpp"
#include "../../src/engine/TableField.cpp"
#include "../../src/engine/NumberField.cpp"
#include "../../src/engine/VarcharField.cpp"
#include "../../src/engine/Table.cpp"
#include "../../src/engine/Engine.cpp"
#include "../../src/engine/DataTypeEnum.h"

using namespace std;

void testCreate() {
    cout << "Test: Engine::create" << endl << endl;

    vector<TableField*> fields;
    fields.push_back(new NumberField("f1"));
    fields.push_back(new VarcharField("f2", 20));
    fields.push_back(new VarcharField("f3", 20));

    Table table("some_table", fields);

    Engine::createTable(table);

    Table table2 = Engine::loadTable("some_table");
    cout << table2 << endl << endl;
}

void testInsert() {
    cout << "Test: Engine::insert" << endl << endl;

    TableRow row;
    row.addValue(new Number(1));
    row.addValue(new Varchar("'v11'"));
    row.addValue(new Varchar("'v12'"));

    vector<TableRow> rows;
    rows.push_back(row);

    TableRow row2;
    row2.addValue(new Number(2));
    row2.addValue(new Varchar("'v21'"));
    row2.addValue(new Varchar("'v22'"));

    rows.push_back(row2);

    TableRow row3;
    row3.addValue(new Number(3));
    row3.addValue(new Varchar("'v31'"));
    row3.addValue(new Varchar("'v32'"));

    rows.push_back(row3);

    TableRow row4;
    row4.addValue(new Number(4));
    row4.addValue(new Varchar("'v41'"));
    row4.addValue(new Varchar("'v22'"));

    rows.push_back(row4);

    Engine::insertIntoTable("some_table", rows);

    Table table2 = Engine::loadTable("some_table", true);

    cout << table2 << endl << endl;
}

void testDelete() {
    TableRow row;
    row.addValue(new Number(1));
    row.addValue(new Varchar("'v11'"));
    row.addValue(new Varchar("'v12'"));

    vector<TableRow> rows;
    rows.push_back(row);

    Engine::deleteFromTable("some_table", rows);

    Table table2 = Engine::loadTable("some_table", true);
    cout << table2 << endl << endl;
}

void testUpdate() {
    vector<TableRow> rows;

    TableRow row2;
    row2.addValue(new Number(2));
    row2.addValue(new Varchar("'v21'"));
    row2.addValue(new Varchar("'v22'"));

    rows.push_back(row2);

    TableRow row4;
    row4.addValue(new Number(4));
    row4.addValue(new Varchar("'v41'"));
    row4.addValue(new Varchar("'v22'"));

    rows.push_back(row4);

    Engine::updateValuesInTable("some_table", rows, new TableField("f3", DataTypeEnum::VARCHAR), new Varchar("'upd'"));

    Table table2 = Engine::loadTable("some_table", true);
    cout << table2 << endl << endl;
}

void testDrop() {
    Engine::deleteTable("some_table");
}

int main() {
    testCreate();
    testInsert();
    testDelete();
    testUpdate();
    testDrop();
}