#include "../../src/engine/DataType.cpp"
#include "../../src/engine/Number.cpp"
#include "../../src/engine/Varchar.cpp"
#include "../../src/engine/TableField.cpp"
#include "../../src/engine/DataTypeEnum.h"

#include "../../src/query_processor/QueryObject.cpp"
#include "../../src/query_processor/InsertObject.cpp"

#include "../../src/query_parser/QueryParser.cpp"

#include "../../src/query_parser/QueryParserException.cpp"

#include <string>

using namespace std;

void testOneField() {
    try {
        string query = "INSERT INTO t ('qwerty');";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

        Varchar firstValue("qwerty", false);

        assert(insertObject->getTable() == "t");
        assert(insertObject->getFieldValues().size() == 1);
        assert(*dynamic_cast<Varchar*>(insertObject->getFieldValues()[0]) == firstValue);
        
        cout << query << endl << insertObject->toString() << endl;
    }
    catch(const exception& e) {
        cout << e.what();
        assert(false);
    }
}

void testMultipleFields() {
    try {
        string query = "INSERT INTO t ('qwerty', 12, '13');";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

        Varchar firstValue("qwerty", false);
        Number secondValue(12);
        Varchar thirdValue("13", false);

        assert(insertObject->getTable() == "t");
        assert(insertObject->getFieldValues().size() == 3);
        assert(*dynamic_cast<Varchar*>(insertObject->getFieldValues()[0]) == firstValue);
        assert(*dynamic_cast<Number*>(insertObject->getFieldValues()[1]) == secondValue);
        assert(*dynamic_cast<Varchar*>(insertObject->getFieldValues()[2]) == thirdValue);
        
        cout << query << endl << insertObject->toString() << endl;
    }
    catch(const exception& e) {
        cout << e.what();
        assert(false);
    }
}

void testWithoutBracesError() {
    try {
        string query = "INSERT INTO t 'qwerty', 12, '13';";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testFirstMissingCommaError() {
    try {
        string query = "INSERT INTO t ('qwerty' 12, '13');";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

        cout << query << endl << insertObject->toString() << endl;
        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testSecondMissingCommaError() {
    try {
        string query = "INSERT INTO t ('qwerty', 12 '13');";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

        cout << query << endl << insertObject->toString() << endl;
        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testMissingMultipleCommaError() {
    try {
        string query = "INSERT INTO t ('qwerty' 12 '13');";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

        cout << query << endl << insertObject->toString() << endl;
        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testMissingRightBraceError() {
    try {
        string query = "INSERT INTO t ('qwerty', 12, '13';";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testMissingLeftBraceError() {
    try {
        string query = "INSERT INTO t 'qwerty', 12, '13');";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        InsertObject* insertObject = dynamic_cast<InsertObject*>(queryObject);

        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

int main() {
    testOneField();
    testMultipleFields();

    testWithoutBracesError();
    testFirstMissingCommaError();
    testSecondMissingCommaError();
    testMissingMultipleCommaError();
    testMissingRightBraceError();
    testMissingLeftBraceError();
}
