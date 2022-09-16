#include "../../src/engine/DataType.cpp"
#include "../../src/engine/Number.cpp"
#include "../../src/engine/Varchar.cpp"
#include "../../src/engine/TableField.cpp"
#include "../../src/engine/DataTypeEnum.h"

#include "../../src/query_processor/QueryObject.cpp"
#include "../../src/query_processor/SelectObject.cpp"

#include "../../src/query_parser/QueryParser.cpp"

#include "../../src/query_parser/QueryParserException.cpp"

#include <string>

using namespace std;

void testValidMultipleFields() {
    try {
        string query = "SELECT f1, f2 FROM qwert WHERE f1 = 5 OR NOT f2 = 'a' AND ( f3 NOT IN ( '1','2','3' ) OR f1 = 2 ) OR f3 NOT LIKE 'b%';";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        SelectObject* selectObject = dynamic_cast<SelectObject*>(queryObject);

        assert(selectObject->getTable() == "qwert");
        assert(selectObject->getFields().size() == 2);
        assert(selectObject->getFields()[0] == "f1");
        assert(selectObject->getFields()[1] == "f2");

        cout << query << endl << selectObject->toString() << endl;
    }
    catch(const exception& e) {
        cout << e.what();
        assert(false);
    }
}

void testValidOneField() {
    try {
        string query = "SELECT f1 FROM qwert WHERE f1 = 5 OR NOT f2 = 'a' AND ( f3 NOT IN ( '1','2','3' ) OR f1 = 2 ) OR f3 NOT LIKE 'b%';";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        SelectObject* selectObject = dynamic_cast<SelectObject*>(queryObject);

        assert(selectObject->getTable() == "qwert");
        assert(selectObject->getFields().size() == 1);
        assert(selectObject->getFields()[0] == "f1");

        cout << query << endl << selectObject->toString() << endl;
    }
    catch(const exception& e) {
        cout << e.what();
        assert(false);
    }
}

void testMissingFields() {
    try {
        string query = "SELECT FROM qwert WHERE f1 = 5 OR NOT f2 = 'a' AND ( f3 NOT IN ( '1','2','3' ) OR f1 = 2 ) OR f3 NOT LIKE 'b%';";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        SelectObject* selectObject = dynamic_cast<SelectObject*>(queryObject);

        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testMissingComma() {
    try {
        string query = "SELECT f1 f2 FROM qwert WHERE f1 = 5 OR NOT f2 = 'a' AND ( f3 NOT IN ( '1','2','3' ) OR f1 = 2 ) OR f3 NOT LIKE 'b%';";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        SelectObject* selectObject = dynamic_cast<SelectObject*>(queryObject);

        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testTwoTableNames() {
    try {
        string query = "SELECT f1, f2 FROM qwert asdf WHERE f1 = 5 OR NOT f2 = 'a' AND ( f3 NOT IN ( '1','2','3' ) OR f1 = 2 ) OR f3 NOT LIKE 'b%';";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        SelectObject* selectObject = dynamic_cast<SelectObject*>(queryObject);

        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

int main() {
    testValidMultipleFields();
    testValidOneField();

    testMissingFields();
    testMissingComma();
    testTwoTableNames();
    
    return 0;
}
