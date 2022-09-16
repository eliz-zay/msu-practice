#include "DeleteQueryResult.h"

#include "QueryResult.h"

DeleteQueryResult::DeleteQueryResult(string table, int time, int rowsNumber): QueryResult(table, time) {
    this->rowsNumber = rowsNumber;
}

string DeleteQueryResult::toString() {
    string result;
    result += "DELETE\n";
    result += "Rows affected: " + to_string(rowsNumber) + "\n";
    result += "----------------\n";
    result += "Execution time: " + to_string(time / 1000.f) + " ms\n";

    return result;
}