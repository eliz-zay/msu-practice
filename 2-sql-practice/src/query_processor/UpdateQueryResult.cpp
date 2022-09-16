#include "UpdateQueryResult.h"

#include "QueryResult.h"

UpdateQueryResult::UpdateQueryResult(string table, int time, int rowsNumber): QueryResult(table, time) {
    this->rowsNumber = rowsNumber;
}

string UpdateQueryResult::toString() {
    string result;
    result += "UPDATE\n";
    result += "Rows affected: " + to_string(rowsNumber) + "\n";
    result += "----------------\n";
    result += "Execution time: " + to_string(time / 1000.f) + " ms\n";

    return result;
}