#include "CreateQueryResult.h"

#include "QueryResult.h"

CreateQueryResult::CreateQueryResult(string table, int time): QueryResult(table, time) {
}

string CreateQueryResult::toString() {
    string result;
    result += "CREATE\n";
    result += "----------------\n";
    result += "Execution time: " + to_string(time / 1000.f) + " ms\n";

    return result;
}