#include "InsertQueryResult.h"

#include "QueryResult.h"

InsertQueryResult::InsertQueryResult(string table, int time, int rowsNumber): QueryResult(table, time) {
    this->rowsNumber = rowsNumber;
}

string InsertQueryResult::toString() {
    string result;
    result += "INSERT\n";
    result += "Rows affected: " + to_string(rowsNumber) + "\n";
    result += "----------------\n";
    result += "Execution time: " + to_string(time / 1000.f) + " ms\n";

    return result;
}