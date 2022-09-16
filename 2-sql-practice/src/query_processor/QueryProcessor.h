#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include "../engine/Table.h"

#include "QueryObject.h"
#include "QueryResult.h"

namespace QueryProcessor {
    QueryResult* executeQuery(QueryObject* queryObject);
    QueryResult* executeSelect(QueryObject* queryObject);
    QueryResult* executeInsert(QueryObject* queryObject);
    QueryResult* executeUpdate(QueryObject* queryObject);
    QueryResult* executeDelete(QueryObject* queryObject);
    QueryResult* executeCreate(QueryObject* queryObject);
    QueryResult* executeDrop(QueryObject* queryObject);
}

#endif