#include "DropQueryResult.h"

#include "QueryResult.h"

DropQueryResult::DropQueryResult(string table, int time): QueryResult(table, time) {
}

string DropQueryResult::toString() {
    string result;
    result += "DROP\n";
    result += "----------------\n";
    result += "Execution time: " + to_string(time / 1000.f) + " ms\n";

    return result;
}