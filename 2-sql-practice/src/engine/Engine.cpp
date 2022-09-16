#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Engine.h"

#include "../shared/VectorHelper.cpp"

#include "Table.cpp"
#include "TableRow.cpp"

#include "TableField.cpp"
#include "NumberField.cpp"
#include "VarcharField.cpp"

#include "DataTypeHelper.cpp"
#include "DataTypeEnum.h"

#include "DataType.cpp"
#include "Varchar.cpp"
#include "Number.cpp"

#include "TableIO.cpp"

#include "EngineStatusEnum.h"
#include "EngineException.cpp"

#include "Config.h"

using namespace std;

/*
 * Creates table with given name and fields
 * @param table - Table object containing name and fields
 * @throws EngineException
 */
void Engine::createTable(Table table) throw(EngineException) {
    int tableFD = TableIO::getFD(table.getName(), TableIO::CREATE_WRITE_MODE);
    
    TableIO::writeFieldsNumber(tableFD, table.getFields().size());

    for (TableField* field : table.getFields()) {
        if (field->getName().size() > TableField::MAX_FIELD_LENGTH) {
            throw EngineException(EngineStatusEnum::TableFieldNameTooLong);
        }
        TableIO::writeTableField(tableFD, field);
    }

    TableIO::closeFD(tableFD);
}

/*
 * Loads table with given name, optionally with rows
 * @param tableName - name of table
 * @param withRows - if true returns table with rows and fields, otherwise only fields
 * @throws EngineException
 * @returns Table object
 */
Table Engine::loadTable(string tableName, bool withRows) throw (EngineException) {
    int tableFD =  TableIO::getFD(tableName, TableIO::READ_MODE);
    int fieldsNumber = TableIO::readFieldsNumber(tableFD);

    vector<TableField*> fields;
    for (int i = 0; i < fieldsNumber; i++) {
        fields.push_back(TableIO::readTableField(tableFD));
    }

    if (!withRows) {
        return Table(tableName, fields);
    }

    vector<TableRow> rows;

    bool isRowFound = true;
    while (isRowFound) {
        int bytesRead = 0;
        TableRow row;
        for (int i = 0; i < fieldsNumber; i++) {
            DataType* value;

            value = TableIO::readTableValue(tableFD, fields[i], bytesRead);
            if (bytesRead == 0 && i == 0) {
                isRowFound = false;
            }
            
            row.addValue(value);
        }
        if (isRowFound) {
            bool deleted = TableIO::readRowDeletedFlag(tableFD);
            if (!deleted) {
                rows.push_back(row);
            }
        }
    }

    TableIO::closeFD(tableFD);

    return Table(tableName, fields, rows);
}


/*
 * Inserts rows into table
 * @param tableName - name of table
 * @param rows - rows to be inserted
 * @throws EngineException
 */
void Engine::insertIntoTable(string tableName, vector<TableRow> rows) throw (EngineException) {
    int tableFD = TableIO::getFD(tableName, TableIO::WRITE_MODE);
    int seekResult = lseek(tableFD, 0, SEEK_END);

    Table table = Engine::loadTable(tableName);
    vector<TableField*> fields = table.getFields();

    for (TableRow row: rows) {
        vector<DataType*> values = row.getValues();

        if (table.getFields().size() != values.size()) {
            throw EngineException(EngineStatusEnum::WrongValuesNumber);
        }

        for (int i = 0; i < values.size(); i++) {
            if (table.getFields()[i]->getType() != values[i]->getType()) {
                throw EngineException(EngineStatusEnum::WrongValueType);
            }

            TableIO::writeTableValue(tableFD, fields[i], values[i]);
        }

        TableIO::writeRowDeletedFlag(tableFD, row.isDeleted());
    }

    TableIO::closeFD(tableFD);
}


/*
 * Updates table rows with given value
 * @param tableName - name of table
 * @param rows - rows to be updated
 * @param field - field to be updated
 * @param value - new value of field
 * @throws EngineException
 */
void Engine::updateValuesInTable(string tableName, vector<TableRow> rows, TableField* field, DataType* value) throw (EngineException) {
    if (field->getType() != value->getType()) {
        throw EngineException(EngineStatusEnum::FieldValueTypesDontMatch);
    }
    
    int tableFD =  TableIO::getFD(tableName, TableIO::READ_WRITE_MODE);
    int fieldsNumber = TableIO::readFieldsNumber(tableFD);

    int headerOffset = 0;
    int bytesRead = 0;

    vector<TableField*> fields;
    for (int i = 0; i < fieldsNumber; i++) {
        fields.push_back(TableIO::readTableField(tableFD, bytesRead));
        headerOffset += bytesRead;
    }

    int fieldIndex = VectorHelper::findInPointerVector(fields, field);
    if (fieldIndex == -1) {
        throw EngineException(EngineStatusEnum::NoSuchField);
    }

    bool isRowFound = true;
    while (isRowFound) {
        TableRow row;
        int rowSize = 0;
        int bytesRead = 0;

        for (int i = 0; i < fieldsNumber; i++) {
            bytesRead = 0;
            DataType* value;

            value = TableIO::readTableValue(tableFD, fields[i], bytesRead);
            if (bytesRead == 0 && i == 0) {
                isRowFound = false;
            }
            rowSize += bytesRead;
            
            row.addValue(value);
        }
        if (isRowFound) {
            bytesRead = 0;
            bool deleted = TableIO::readRowDeletedFlag(tableFD, bytesRead);
            rowSize += bytesRead;

            if (VectorHelper::findInVector(rows, row) != -1) {
                row.setValue(fieldIndex, value);

                TableIO::seek(tableFD, TableIO::CURRENT_WHENCE, -rowSize);

                vector<DataType*> values = row.getValues();
                for (int i = 0; i < values.size(); i++) {
                    TableIO::writeTableValue(tableFD, fields[i], values[i]);
                }
                TableIO::writeRowDeletedFlag(tableFD, row.isDeleted());
            }
        }
    }

    TableIO::closeFD(tableFD);
}

/*
 * Deletes given rows from table
 * @param tableName - name of table
 * @param rows - rows to be deleted
 * @throws EngineException
 */
void Engine::deleteFromTable(string tableName, vector<TableRow> rows) throw (EngineException) {
    int tableFD =  TableIO::getFD(tableName, TableIO::READ_WRITE_MODE);
    
    int fieldsNumber = TableIO::readFieldsNumber(tableFD);

    int headerOffset = 0;
    int bytesRead = 0;

    vector<TableField*> fields;
    for (int i = 0; i < fieldsNumber; i++) {
        fields.push_back(TableIO::readTableField(tableFD, bytesRead));
        headerOffset += bytesRead;
    }

    bool isRowFound = true;
    while (isRowFound) {
        TableRow row;
        for (int i = 0; i < fieldsNumber; i++) {
            int bytesRead = 0;
            DataType* value;

            value = TableIO::readTableValue(tableFD, fields[i], bytesRead);
            if (bytesRead == 0 && i == 0) {
                isRowFound = false;
            }
            
            row.addValue(value);
        }
        if (isRowFound) {
            bool deleted = TableIO::readRowDeletedFlag(tableFD);
            if (!deleted && VectorHelper::findInVector(rows, row) != -1) {
                TableIO::seek(tableFD, TableIO::CURRENT_WHENCE, -sizeof(bool));
                TableIO::writeRowDeletedFlag(tableFD, true);
            }
        }
    }

    TableIO::closeFD(tableFD);
}

/*
 * Deletes table with given name
 * @param table - table name to be deleted
 */
void Engine::deleteTable(string table) throw (EngineException) {
    TableIO::deleteTable(table);
}