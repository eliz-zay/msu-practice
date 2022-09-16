#pragma once

#include "QueryResult.h"

QueryResult::QueryResult(string table, int time) {
    this->table = table;
    this->time = time;
}