#include "../../src/engine/DataType.cpp"
#include "../../src/engine/Number.cpp"
#include "../../src/engine/Varchar.cpp"
#include "../../src/engine/TableField.cpp"
#include "../../src/engine/DataTypeEnum.h"

#include "../../src/query_processor/QueryObject.cpp"
#include "../../src/query_processor/DropObject.cpp"

#include "../../src/query_parser/QueryParser.cpp"

#include "../../src/query_parser/QueryParserException.cpp"

#include <string>

using namespace std;

void testValid() {
    try {
        string query = "DROP TABLE qwert;";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        DropObject* dropObject = dynamic_cast<DropObject*>(queryObject);

        assert(dropObject->getTable() == "qwert");

        cout << query << endl << dropObject->toString() << endl;
    }
    catch(const exception& e) {
        cout << e.what();
        assert(false);
    }
}

void testMissingTableKeyword() {
    try {
        string query = "DROP qwert;";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        DropObject* dropObject = dynamic_cast<DropObject*>(queryObject);

        assert(true);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testMissingTableName() {
    try {
        string query = "DROP TABLE;";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        DropObject* dropObject = dynamic_cast<DropObject*>(queryObject);

        assert(true);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testWrongTableName() {
    try {
        string query = "DROP TABLE (qwert);";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        DropObject* dropObject = dynamic_cast<DropObject*>(queryObject);

        assert(true);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

int main() {
    testValid();

    testMissingTableKeyword();
    testMissingTableName();
    testWrongTableName();
}
