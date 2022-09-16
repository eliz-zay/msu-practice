#include "../../src/engine/DataType.cpp"
#include "../../src/engine/Number.cpp"
#include "../../src/engine/Varchar.cpp"
#include "../../src/engine/TableField.cpp"
#include "../../src/engine/DataTypeEnum.h"

#include "../../src/query_processor/QueryObject.cpp"
#include "../../src/query_processor/CreateObject.cpp"

#include "../../src/query_parser/QueryParser.cpp"

#include "../../src/query_parser/QueryParserException.cpp"

#include <string>

using namespace std;

void testValidCreateSingleField() {
    try {
        string query = "CREATE TABLE t (cat VARCHAR (12));";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        CreateObject* createObject = dynamic_cast<CreateObject*>(queryObject);
        
        vector<TableField*> tableFields;
        
        tableFields.push_back(new TableField("cat", DataTypeEnum::VARCHAR));
        
        for (int i = 0; i < tableFields.size() && i < createObject->getTableFields().size(); ++i) {
            assert(*(createObject->getTableFields())[i] == *(tableFields[i]));
        }
        
        assert(queryObject->getTable() == "t");
        
        cout << query << endl << createObject->toString() << endl;
        
        query = "CREATE TABLE t (lama NUMBER);";
        queryObject = QueryParser::parseQuery(query);
        createObject = dynamic_cast<CreateObject*>(queryObject);
        
        tableFields.clear();
        tableFields.push_back(new TableField("lama", DataTypeEnum::NUMBER));
        
        for (int i = 0; i < tableFields.size() && i < createObject->getTableFields().size(); ++i) {
            assert(*(createObject->getTableFields())[i] == *(tableFields[i]));
        }
        
        assert(queryObject->getTable() == "t");
        
        cout << query << endl << createObject->toString() << endl;
    } catch (exception &ex) {
        assert(false);
    }
}

void testValidCreateNumberFields() {
    try {
        string query = "CREATE TABLE t (cat NUMBER, doggo NUMBER, lama NUMBER);";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        CreateObject* createObject = dynamic_cast<CreateObject*>(queryObject);
        
        vector<TableField*> tableFields;
        
        tableFields.push_back(new TableField("cat", DataTypeEnum::NUMBER));
        tableFields.push_back(new TableField("doggo", DataTypeEnum::NUMBER));
        tableFields.push_back(new TableField("lama", DataTypeEnum::NUMBER));
        
        for (int i = 0; i < tableFields.size() && i < createObject->getTableFields().size(); ++i) {
            assert(*(createObject->getTableFields())[i] == *(tableFields[i]));
        }
        
        assert(queryObject->getTable() == "t");
        
        cout << query << endl << createObject->toString() << endl;
    } catch (exception &ex) {
        assert(false);
    }
}

void testValidCreateVarcharFields() {
    try {
        string query = "CREATE TABLE t (cat VARCHAR (10), doggo VARCHAR (12), lama VARCHAR (14));";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        CreateObject* createObject = dynamic_cast<CreateObject*>(queryObject);
        
        vector<TableField*> tableFields;
        
        tableFields.push_back(new TableField("cat", DataTypeEnum::VARCHAR));
        tableFields.push_back(new TableField("doggo", DataTypeEnum::VARCHAR));
        tableFields.push_back(new TableField("lama", DataTypeEnum::VARCHAR));
        
        for (int i = 0; i < tableFields.size() && i < createObject->getTableFields().size(); ++i) {
            assert(*(createObject->getTableFields())[i] == *(tableFields[i]));
        }
        
        assert(queryObject->getTable() == "t");
        
        cout << query << endl << createObject->toString() << endl;
    } catch (exception &ex) {
        assert(false);
    }
}

void testValidCreateMixedFields() {
    try {
        string query = "CREATE TABLE t (cat NUMBER, doggo VARCHAR (12), lama NUMBER);";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        CreateObject* createObject = dynamic_cast<CreateObject*>(queryObject);
        
        vector<TableField*> tableFields;
        
        tableFields.push_back(new TableField("cat", DataTypeEnum::NUMBER));
        tableFields.push_back(new TableField("doggo", DataTypeEnum::VARCHAR));
        tableFields.push_back(new TableField("lama", DataTypeEnum::NUMBER));
        
        for (int i = 0; i < tableFields.size() && i < createObject->getTableFields().size(); ++i) {
            assert(*(createObject->getTableFields())[i] == *(tableFields[i]));
        }
        
        assert(queryObject->getTable() == "t");
        
        cout << query << endl << createObject->toString() << endl;
    } catch (exception &ex) {
        assert(false);
    }
}

void testWrongTableName() {
    try {
        string query = "CREATE TABLE 'table' (cat NUMBER, doggo VARCHAR (12), lama NUMBER);";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
    
    try {
        string query = "CREATE TABLE 134 (cat NUMBER, doggo VARCHAR (12), lama NUMBER);";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}

void testWrongFieldsDescritpion() {
    try {
        string query = "CREATE TABLE table (doggo VARCHAR, lama NUMBER);";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
    
    try {
        string query = "CREATE TABLE table (doggo VARCHAR (), lama NUMBER);";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
    
    try {
        string query = "CREATE TABLE table (doggo VARCHAR (34), lama);";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}

void testMissingBrackets() {
    try {
        string query = "CREATE TABLE table doggo VARCHAR (12), lama NUMBER;";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }
}

int main() {
    testValidCreateSingleField();
    testValidCreateNumberFields();
    testValidCreateVarcharFields();
    testValidCreateMixedFields();

    testWrongTableName();
    testWrongFieldsDescritpion();
    testMissingBrackets();
    
    return 0;
}
