#include "../../src/engine/DataType.cpp"
#include "../../src/engine/Number.cpp"
#include "../../src/engine/Varchar.cpp"
#include "../../src/engine/TableField.cpp"
#include "../../src/engine/DataTypeEnum.h"

#include "../../src/query_processor/QueryObject.cpp"
#include "../../src/query_processor/UpdateObject.cpp"

#include "../../src/query_parser/QueryParser.cpp"

#include "../../src/query_parser/QueryParserException.cpp"

#include <string>

using namespace std;

void testValidUpdateWithWhere() {
    try {
        string query = "UPDATE t SET f1='a' WHERE f2=5 AND f1='b';";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        UpdateObject* updateObject = dynamic_cast<UpdateObject*>(queryObject);

        assert(updateObject->getField()->getName() == "f1");
        assert(updateObject->getField()->getType() == DataTypeEnum::VARCHAR);

        Varchar* value = dynamic_cast<Varchar*>(updateObject->getValue());
        assert(value->getValue() == "a");

        assert(updateObject->getConditionTree() != nullptr);
        assert(queryObject->getTable() == "t");
        
        cout << query << endl << updateObject->toString() << endl;
    }
    catch(const exception& e) {
        assert(false);
    }
}

void testValidUpdateWithoutWhere() {
    try {
        string query = "UPDATE t SET f1='a';";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        UpdateObject* updateObject = dynamic_cast<UpdateObject*>(queryObject);

        assert(updateObject->getField()->getName() == "f1");
        assert(updateObject->getField()->getType() == DataTypeEnum::VARCHAR);

        Varchar* value = dynamic_cast<Varchar*>(updateObject->getValue());
        assert(value->getValue() == "a");

        assert(updateObject->getConditionTree() == nullptr);
        assert(queryObject->getTable() == "t");

        cout << query << endl << updateObject->toString() << endl;
    }
    catch(const exception& e) {
        assert(false);
    }
}

void testValidMixedCaseKeywords() {
    try {
        string query = "uPdAtE t set f1='a';";
        QueryObject* queryObject = QueryParser::parseQuery(query);
        UpdateObject* updateObject = dynamic_cast<UpdateObject*>(queryObject);

        assert(updateObject->getField()->getName() == "f1");
        assert(updateObject->getField()->getType() == DataTypeEnum::VARCHAR);

        Varchar* value = dynamic_cast<Varchar*>(updateObject->getValue());
        assert(value->getValue() == "a");

        assert(updateObject->getConditionTree() == nullptr);
        assert(queryObject->getTable() == "t");

        cout << query << endl << updateObject->toString() << endl;
    }
    catch(const exception& e) {
        assert(false);
    }
}

void testMissingSemicolon() {
    try {
        string query = "UPDATE t SET f1='a'";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testWrongTableName() {
    try {
        string query = "UPDATE 1table SET f1='a';";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

void testWrongFieldName() {
    try {
        string query = "UPDATE t SET 1f='a';";
        cout << query << endl;
        QueryObject* queryObject = QueryParser::parseQuery(query);
        assert(false);
    }
    catch(const exception& e) {
        cout << e.what() << endl;
        assert(true);
    }
}

int main() {
    testValidUpdateWithWhere();
    testValidUpdateWithoutWhere();
    testValidMixedCaseKeywords();

    testMissingSemicolon();
    testWrongTableName();
    testWrongFieldName();
}
