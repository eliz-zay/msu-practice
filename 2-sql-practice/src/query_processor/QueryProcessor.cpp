#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

#include "QueryProcessor.h"

#include "../engine/Table.cpp"
#include "../engine/TableRow.cpp"
#include "../engine/DataType.cpp"
#include "../engine/TableField.cpp"
#include "../engine/Engine.cpp"
#include "../engine/EngineException.cpp"
#include "../engine/EngineStatusEnum.h"

#include "QueryObject.cpp"
#include "SelectObject.cpp"
#include "QueryTypeEnum.h"
#include "QueryResult.cpp"
#include "SelectQueryResult.cpp"
#include "InsertQueryResult.cpp"
#include "UpdateQueryResult.cpp"
#include "DeleteQueryResult.cpp"
#include "CreateQueryResult.cpp"
#include "DropQueryResult.cpp"

#include "condition_tree/BaseCondition.h"
#include "condition_tree/BinaryCondition.cpp"
#include "condition_tree/InCondition.cpp"
#include "condition_tree/BaseOperand.cpp"
#include "condition_tree/TableFieldOperand.cpp"
#include "condition_tree/NumberSetOperand.cpp"

#include "../shared/VectorHelper.cpp"

using namespace std;

/*
 * Executes query
 * @params queryObject - object which contains parsed inforamtion about query
 * @returns string
 */
QueryResult* QueryProcessor::executeQuery(QueryObject* queryObject) {
    switch (queryObject->getType()) {
        case QueryTypeEnum::Select:
            return QueryProcessor::executeSelect(queryObject);
        case QueryTypeEnum::Insert:
            return QueryProcessor::executeInsert(queryObject);
        case QueryTypeEnum::Update:
            return QueryProcessor::executeUpdate(queryObject);
        case QueryTypeEnum::Delete:
            return QueryProcessor::executeDelete(queryObject);
        case QueryTypeEnum::Create:
            return QueryProcessor::executeCreate(queryObject);
        case QueryTypeEnum::Drop:
            return QueryProcessor::executeDrop(queryObject);
    }
}

/*
 * Executes SELECT query
 * @params queryObject - object which contains parsed inforamtion about query
 * @returns string
 */
QueryResult* QueryProcessor::executeSelect(QueryObject* queryObject) {
    SelectObject* selectObject = dynamic_cast<SelectObject*>(queryObject);

    // start time
    auto start = chrono::high_resolution_clock::now();

    Table table = Engine::loadTable(queryObject->getTable(), true);

    vector<TableField*> fields;
    vector<bool> fieldsPresence(table.getFields().size(), false);

    if (selectObject->getFields().size() == 1 && selectObject->getFields()[0] == "*") {
        fields = table.getFields();
        fieldsPresence = vector<bool>(table.getFields().size(), true);
    } else {
        for (string fieldString: selectObject->getFields()) {
            bool found = false;
            for (int i = 0; i < table.getFields().size(); i++) {
                if (fieldString == table.getFields()[i]->getName()) {
                    fields.push_back(table.getFields()[i]);
                    fieldsPresence[i] = true;
                    found = true;
                    continue;
                }
            }
            if (!found) {
                throw EngineException(EngineStatusEnum::NoSuchField);
            }
        }
    }

    vector<vector<DataType*> > rows;

    for (TableRow row: table.getRows()) {
        vector<DataType*> resultRow;
        if (!selectObject->getConditionTree() || selectObject->getConditionTree()->calculate(table.getFields(), row.getValues())) {
            for (int i = 0; i < row.getValues().size(); i++) {
                if (fieldsPresence[i]) {
                    resultRow.push_back(row.getValues()[i]);
                }
            }
            rows.push_back(resultRow);
        }
    }

    // end time
    auto end = chrono::high_resolution_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(end - start).count();

    return new SelectQueryResult(selectObject->getTable(), time, fields, rows);
}

QueryResult* QueryProcessor::executeInsert(QueryObject* queryObject) {
    InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

    // start time
    auto start = chrono::high_resolution_clock::now();
    
    Engine::insertIntoTable(insertObject->getTable(), vector<TableRow>(1, TableRow(insertObject->getFieldValues())));

    // end time
    auto end = chrono::high_resolution_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(end - start).count();

    return new InsertQueryResult(insertObject->getTable(), time, 1);
}

QueryResult* QueryProcessor::executeUpdate(QueryObject* queryObject) {
    UpdateObject* updateObject = dynamic_cast<UpdateObject*>(queryObject);

    // start time
    auto start = chrono::high_resolution_clock::now();
    
    Table table = Engine::loadTable(updateObject->getTable(), true);

    vector<TableRow> rows;
    for (TableRow row: table.getRows()) {
        if (!updateObject->getConditionTree() || updateObject->getConditionTree()->calculate(table.getFields(), row.getValues())) {
            rows.push_back(row);
        }
    }

    Engine::updateValuesInTable(updateObject->getTable(), rows, updateObject->getField(), updateObject->getValue());

    // end time
    auto end = chrono::high_resolution_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(end - start).count();

    return new UpdateQueryResult(updateObject->getTable(), time, rows.size());
}

QueryResult* QueryProcessor::executeDelete(QueryObject* queryObject) {
    DeleteObject* deleteObject = dynamic_cast<DeleteObject*>(queryObject);

    // start time
    auto start = chrono::high_resolution_clock::now();

    Table table = Engine::loadTable(deleteObject->getTable(), true);

    vector<TableRow> rows;
    for (TableRow row: table.getRows()) {
        if (!deleteObject->getConditionTree() || deleteObject->getConditionTree()->calculate(table.getFields(), row.getValues())) {
            rows.push_back(row);
        }
    }

    Engine::deleteFromTable(deleteObject->getTable(), rows);

    // end time
    auto end = chrono::high_resolution_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(end - start).count();

    return new DeleteQueryResult(deleteObject->getTable(), time, rows.size());
}

QueryResult* QueryProcessor::executeCreate(QueryObject* queryObject) {
    CreateObject* createObject = dynamic_cast<CreateObject*>(queryObject);

    // start time
    auto start = chrono::high_resolution_clock::now();

    Engine::createTable(Table(createObject->getTable(), createObject->getTableFields()));

    // end time
    auto end = chrono::high_resolution_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(end - start).count();

    return new CreateQueryResult(createObject->getTable(), time);
}

QueryResult* QueryProcessor::executeDrop(QueryObject* queryObject) {
    DropObject* dropObject = dynamic_cast<DropObject*>(queryObject);

    // start time
    auto start = chrono::high_resolution_clock::now();

    Engine::deleteTable(dropObject->getTable());

    // end time
    auto end = chrono::high_resolution_clock::now();
    int time = chrono::duration_cast<chrono::microseconds>(end - start).count();

    return new DropQueryResult(dropObject->getTable(), time);
}